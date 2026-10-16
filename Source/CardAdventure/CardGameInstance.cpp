#include "CardGameInstance.h"
#include "CardManager.h"
#include "Math/UnrealMathUtility.h" // FMath 사용을 위해 포함

void UCardGameInstance::InitializePlayer(EPlayerClass CharClass, TSubclassOf<USkillBase> StartingSkillClass)
{
	CurrentPlayer.PlayerClass = CharClass;

	// 기존 데이터 초기화
	CurrentPlayer.MaxMana.Clear();
	CurrentPlayer.Deck.Empty();

	// 직업별 초기 스탯 및 덱 세팅
	switch (CharClass)
	{
	case EPlayerClass::Knight:
		CurrentPlayer.MaxHP = 80;
		CurrentPlayer.CurrentHP = 80;
		CurrentPlayer.Gold = 99;
		CurrentPlayer.MaxMana.Red = 2;
		CurrentPlayer.MaxMana.Colorless = 1;

		for (int32 i = 0; i < 5; i++)
		{
			FCardInstance StrikeCard; StrikeCard.CardID = FName("K_Strike"); CurrentPlayer.Deck.Add(StrikeCard);
			FCardInstance DefendCard; DefendCard.CardID = FName("K_Defend"); CurrentPlayer.Deck.Add(DefendCard);
		}
		break;

	case EPlayerClass::Rogue:
		CurrentPlayer.MaxHP = 70;
		CurrentPlayer.CurrentHP = 70;
		CurrentPlayer.Gold = 99;
		CurrentPlayer.MaxMana.Green = 2;
		CurrentPlayer.MaxMana.Colorless = 1;

		for (int32 i = 0; i < 5; i++)
		{
			FCardInstance StrikeCard; StrikeCard.CardID = FName("R_Strike"); CurrentPlayer.Deck.Add(StrikeCard);
			FCardInstance DefendCard; DefendCard.CardID = FName("R_Defend"); CurrentPlayer.Deck.Add(DefendCard);
		}
		break;
	}

	// ★ 매개변수로 받은 스킬 클래스를 세팅하고 즉시 객체 생성
	ActiveSkillClass = StartingSkillClass;
	InitializeSkill();

	// 모든 세팅 완료 후 신호 발송
	OnPlayerStatChanged.Broadcast();
}

void UCardGameInstance::GenerateTestMap()
{
	MapNodes.Empty();
	int32 NodeIDCounter = 0;

	// 각 Layer(열)마다 속한 노드들의 ID를 임시 저장할 2차원 배열
	TArray<TArray<int32>> LayerNodes;

	// 1. 노드 생성 및 위치 설정
	for (int32 LayerIndex = 0; LayerIndex < TotalLayers; ++LayerIndex)
	{
		TArray<int32> CurrentLayerIDs;

		// 층마다 2~3개의 갈래 생성 (단, 마지막 층(보스)은 무조건 1개)
		int32 NumNodesInLayer = (LayerIndex == 0 || LayerIndex == TotalLayers - 1) ? 1 : FMath::RandRange(2, 3);

		// 노드들이 화면 중앙을 기준으로 상하 정렬되도록 시작 Y좌표 계산
		float StartY = 540.0f + ((NumNodesInLayer - 1) * NodeSpacingY * -0.5f);

		for (int32 i = 0; i < NumNodesInLayer; ++i)
		{
			FMapNode NewNode;
			NewNode.NodeID = NodeIDCounter++;

			// X좌표: 층이 오를수록 우측으로 이동 (여백 150 확보)
			// Y좌표: StartY를 기준으로 간격만큼 아래로 배치 (가운데 정렬)
			NewNode.Position = FVector2D(150.0f + (LayerIndex * NodeSpacingX), StartY + (i * NodeSpacingY));

			// ★ 수정된 부분: RoomType -> NodeType, ERoomType -> ENodeType
			if (LayerIndex == 0)
			{
				// 고대의 존재(시작) 노드
				NewNode.NodeType = ENodeType::Unknown;
			}
			else if (LayerIndex == 1)
			{
				NewNode.NodeType = ENodeType::Shop; // 테스트용 첫방 고정
			}
			else if (LayerIndex == TotalLayers - 2)
			{
				NewNode.NodeType = ENodeType::Rest; // 마지막전은 휴식
			}
			else if (LayerIndex == TotalLayers - 1)
			{
				NewNode.NodeType = ENodeType::Boss; // 마지막은 보스
			}
			else
			{
				// 전투, 엘리트, 휴식, 상점, 이벤트 중 랜덤 배정 (0 ~ 4)
				int32 RandomNodeIndex = FMath::RandRange(0, 4);
				NewNode.NodeType = static_cast<ENodeType>(RandomNodeIndex);
			}

			MapNodes.Add(NewNode);
			CurrentLayerIDs.Add(NewNode.NodeID);
		}
		LayerNodes.Add(CurrentLayerIDs);
	}

	// 2. 노드 간의 경로 연결 (좌측 층에서 우측 층으로 - 선 꼬임 방지 알고리즘 적용)
	for (int32 LayerIndex = 0; LayerIndex < TotalLayers - 1; ++LayerIndex)
	{
		TArray<int32> &CurrentLayer = LayerNodes[LayerIndex];
		TArray<int32> &NextLayer = LayerNodes[LayerIndex + 1];

		// 양쪽 층의 위에서(인덱스 0)부터 시작
		int32 CurrIdx = 0;
		int32 NextIdx = 0;

		// 두 층 중 하나라도 끝에 도달할 때까지 반복
		while (CurrIdx < CurrentLayer.Num() && NextIdx < NextLayer.Num())
		{
			// 1. 현재 가리키고 있는 두 노드를 연결
			int32 CurrNodeID = CurrentLayer[CurrIdx];
			int32 NextNodeID = NextLayer[NextIdx];
			MapNodes[CurrNodeID].ConnectedNodes.AddUnique(NextNodeID);

			// 2. 다음으로 이동할 포인터 결정
			bool bCanAdvanceCurr = (CurrIdx < CurrentLayer.Num() - 1);
			bool bCanAdvanceNext = (NextIdx < NextLayer.Num() - 1);

			if (bCanAdvanceCurr && bCanAdvanceNext)
			{
				// 둘 다 다음 노드가 있다면 무작위로 진행 방향 결정 (0: 현재 층만, 1: 다음 층만, 2: 둘 다)
				int32 RandomChoice = FMath::RandRange(0, 2);
				if (RandomChoice == 0)
				{
					CurrIdx++;
				}
				else if (RandomChoice == 1)
				{
					NextIdx++;
				}
				else
				{
					CurrIdx++;
					NextIdx++;
				}
			}
			else if (bCanAdvanceCurr)
			{
				// 현재 층만 남았다면 현재 층 포인터만 이동
				CurrIdx++;
			}
			else if (bCanAdvanceNext)
			{
				// 다음 층만 남았다면 다음 층 포인터만 이동
				NextIdx++;
			}
			else
			{
				// 둘 다 끝에 도달했으면 종료
				break;
			}
		}
	}

	// 초기 설정 완료
	CurrentNodeID = 0;
	VisitedNodes.Empty(); // 맵을 새로 만들 때 지나온 길도 초기화!
	UE_LOG(LogTemp, Log, TEXT("[Map System] Random Map Generated (Left to Right)!"));
}

bool UCardGameInstance::IsNodeAccessible(int32 TargetNodeID) const
{
	// 클리어시만 활성화
	if (!IsNodeCleared)
	{
		return false;
	}

	// 2. 이미 특정 노드에 위치해 있다면?
	if (MapNodes.IsValidIndex(CurrentNodeID))
	{
		// 현재 내 노드의 'ConnectedNodes(연결된 선 배열)' 안에 타겟 노드가 있는지 확인
		return MapNodes[CurrentNodeID].ConnectedNodes.Contains(TargetNodeID);
	}

	return false;
}

// 2. 이동할 때 지나온 길 기록하기 (MoveToNode 함수 수정)
void UCardGameInstance::MoveToNode(int32 TargetNodeID)
{
	if (IsNodeAccessible(TargetNodeID))
	{
		// ★ 방을 이동하기 전에, 기존에 있던 방을 '지나온 길' 배열에 추가
		if (CurrentNodeID != -1)
		{
			VisitedNodes.AddUnique(CurrentNodeID);
		}

		CurrentNodeID = TargetNodeID;
		IsNodeCleared = false;

		UE_LOG(LogTemp, Log, TEXT("[Map] Player moved to Node ID: %d"), CurrentNodeID);

		OnNodeMoved.Broadcast();
	}
}

// 3. 상태 판별 함수 추가 (새로 작성)
EMapNodeState UCardGameInstance::GetNodeState(int32 TargetNodeID) const
{
	// 1단계: 이미 클리어하고 지나온 방인가?
	if (VisitedNodes.Contains(TargetNodeID))
	{
		return EMapNodeState::Cleared;
	}

	// 2단계: 플레이어가 지금 서 있는 방인가?
	if (TargetNodeID == CurrentNodeID)
	{
		return EMapNodeState::Current;
	}

	// 3단계: 이동 가능한 다음 방인가? (기존에 만든 IsNodeAccessible 활용)
	if (IsNodeAccessible(TargetNodeID))
	{
		return EMapNodeState::Accessible;
	}

	// 4단계: 위 조건에 모두 안 맞으면 아직 못 가는 방 (비활성)
	return EMapNodeState::Locked;
}

void UCardGameInstance::CompleteCurrentNode()
{
	// ★ 방을 클리어했으므로 자물쇠를 풂
	IsNodeCleared = true;

	// 자물쇠가 풀렸으니 다음 길에 불이 들어오도록 맵 노드들에게 새로고침 알람 발송!
	OnNodeMoved.Broadcast();
}

void UCardGameInstance::InitializeSkill()
{
	// 에디터에서 스킬 클래스가 지정되어 있다면
	if (ActiveSkillClass)
	{
		// 이 인스턴스를 부모로 하여 스킬 객체를 생성합니다.
		ActiveSkillInstance = NewObject<USkillBase>(this, ActiveSkillClass);
		UE_LOG(LogTemp, Log, TEXT("직업 스킬 장착 완료!"));
	}
}

void UCardGameInstance::UpdateSkillUI(int32 CurrentCharge, int32 MaxCharge, bool bIsReady)
{
	// 1. 게이지 바 갱신 신호 발송
	OnSkillChargeChanged.Broadcast(CurrentCharge, MaxCharge);

	// 2. 꽉 찼다면 발동 가능(버튼 반짝임 등) 신호 발송
	if (bIsReady)
	{
		OnSkillReady.Broadcast();
	}
}

void UCardGameInstance::AddGold(int32 Amount)
{
	// 골드 증감 적용 (0 밑으로 떨어지지 않게 Clamp/Max 처리)
	CurrentPlayer.Gold = FMath::Max(0, CurrentPlayer.Gold + Amount);

	UE_LOG(LogTemp, Log, TEXT("[Player Data] Gold changed by %d. Current Gold: %d"), Amount, CurrentPlayer.Gold);

	// ★ 골드가 변했으니 UI 업데이트 신호 발송! (이미 만들어두신 델리게이트 활용)
	OnPlayerStatChanged.Broadcast();
}

void UCardGameInstance::AddCardToDeck(FName NewCardID)
{
	// 1. 새로운 카드 인스턴스 생성
	FCardInstance NewCard;
	NewCard.CardID = NewCardID;

	// 2. 현재 플레이어의 덱 배열에 추가
	CurrentPlayer.Deck.Add(NewCard);

	UE_LOG(LogTemp, Log, TEXT("[Player Data] Card Added to Deck: %s"), *NewCardID.ToString());
}

void UCardGameInstance::ApplyEventReward(EEventRewardType RewardType, int32 RewardValue)
{
	switch (RewardType)
	{
	case EEventRewardType::TakeDamage:
		// 체력 감소 (0 이하로 떨어지지 않게 Clamp)
		CurrentPlayer.CurrentHP = FMath::Clamp(CurrentPlayer.CurrentHP - RewardValue, 0, CurrentPlayer.MaxHP);
		UE_LOG(LogTemp, Log, TEXT("[Event] Take Damage: %d, Current HP: %d"), RewardValue, CurrentPlayer.CurrentHP);
		break;

	case EEventRewardType::HealHP:
		// 체력 회복 (최대 체력을 넘지 않게 Clamp)
		CurrentPlayer.CurrentHP = FMath::Clamp(CurrentPlayer.CurrentHP + RewardValue, 0, CurrentPlayer.MaxHP);
		UE_LOG(LogTemp, Log, TEXT("[Event] Heal HP: %d, Current HP: %d"), RewardValue, CurrentPlayer.CurrentHP);
		break;

	case EEventRewardType::GainGold:
		// 골드 획득
		AddGold(RewardValue);
		UE_LOG(LogTemp, Log, TEXT("[Event] Gain Gold: %d, Total Gold: %d"), RewardValue, CurrentPlayer.Gold);
		break;

	case EEventRewardType::LoseGold:
		// 골드 감소 (0 이하로 떨어지지 않게 Max 사용)
		CurrentPlayer.Gold = FMath::Max(0, CurrentPlayer.Gold - RewardValue);
		UE_LOG(LogTemp, Log, TEXT("[Event] Lose Gold: %d, Total Gold: %d"), RewardValue, CurrentPlayer.Gold);
		break;

	case EEventRewardType::None:
	default:
		// 아무 일도 없음 즉시 함수 종료
		UE_LOG(LogTemp, Log, TEXT("[Event] No Reward / Effect."));
		return;
	}

	// ★ 스위치 문이 끝나고 스탯이 변했다면, UI 새로고침 신호 발송!
	OnPlayerStatChanged.Broadcast();
}

void UCardGameInstance::RestAtCampfire()
{
	// 1. 최대 체력의 30%를 계산하고 소수점을 버림 (TruncToInt 사용)
	int32 HealAmount = FMath::TruncToInt(CurrentPlayer.MaxHP * 0.3f);

	// 2. 현재 체력에 더한 뒤, 최대 체력을 넘지 않도록 Clamp 적용
	CurrentPlayer.CurrentHP = FMath::Clamp(CurrentPlayer.CurrentHP + HealAmount, 0, CurrentPlayer.MaxHP);

	UE_LOG(LogTemp, Log, TEXT("[Rest Node] Player rested. Healed %d HP. Current HP: %d / %d"), HealAmount, CurrentPlayer.CurrentHP, CurrentPlayer.MaxHP);

	// 3. 체력이 변했으니 UI 업데이트 신호 발송
	OnPlayerStatChanged.Broadcast();
}