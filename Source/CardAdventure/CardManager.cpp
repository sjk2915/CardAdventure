#include "CardManager.h"
#include "CardCharacter.h"
#include "EnemyCharacter.h"
#include "CardGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UCardManager::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);

	FString DataTablePath = TEXT("/Game/Data/DT_CardData.DT_CardData");

	UDataTable *LoadedTable = LoadObject<UDataTable>(nullptr, *DataTablePath);
	if (LoadedTable)
	{
		CardDataTable = LoadedTable;
		UE_LOG(LogTemp, Log, TEXT("카드 데이터 테이블 C++ 로드 성공!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("카드 데이터 테이블을 찾을 수 없습니다! 경로(%s)를 확인하세요."), *DataTablePath);
	}

	UE_LOG(LogTemp, Log, TEXT("[CardManager] Subsystem Initialized."));
}

void UCardManager::Deinitialize()
{
	Super::Deinitialize();
}

void UCardManager::StartBattle()
{
	UCardGameInstance *GameInst = Cast<UCardGameInstance>(GetGameInstance());
	if (GameInst)
	{
		Deck = GameInst->CurrentPlayer.Deck;

		if (UWorld *World = GameInst->GetWorld())
		{
			AActor *PlayerActor = UGameplayStatics::GetActorOfClass(World, ACardCharacter::StaticClass());
			if (ACardCharacter *PlayerCharacter = Cast<ACardCharacter>(PlayerActor))
			{
				PlayerCharacter->InitStats(GameInst->CurrentPlayer.MaxHP, GameInst->CurrentPlayer.CurrentHP);
			}
		}
	}

	Hand.Empty();
	DiscardPile.Empty();
	DrawPile = Deck;

	for (int32 i = DrawPile.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		DrawPile.Swap(i, j);
	}

	UpdateDeckCounts();
	ResetMana();
	DrawCards(5);
}

void UCardManager::DrawCards(int32 Amount)
{
	for (int32 i = 0; i < Amount; ++i)
	{
		if (DrawPile.Num() == 0) ShuffleDiscardIntoDraw();
		if (DrawPile.Num() == 0) break;

		FCardInstance DrawnCard = DrawPile.Pop();
		Hand.Add(DrawnCard);
	}

	OnHandUpdated.Broadcast();
	UpdateDeckCounts();
}

void UCardManager::ShuffleDiscardIntoDraw()
{
	if (DiscardPile.Num() == 0) return;

	DrawPile.Append(DiscardPile);
	DiscardPile.Empty();

	for (int32 i = DrawPile.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		DrawPile.Swap(i, j);
	}
}

void UCardManager::DiscardCard(int32 HandIndex)
{
	if (Hand.IsValidIndex(HandIndex))
	{
		FCardInstance CardToDiscard = Hand[HandIndex];
		Hand.RemoveAt(HandIndex);
		DiscardPile.Add(CardToDiscard);

		OnHandUpdated.Broadcast();
		UpdateDeckCounts();
	}
}

void UCardManager::DiscardHand()
{
	DiscardPile.Append(Hand);
	Hand.Empty();
	OnHandUpdated.Broadcast();
	UpdateDeckCounts();
}

bool UCardManager::ExecuteCard(const FCardData &InCardData, int32 HandIndex, ACardCharacter *InstigatorCharacter, ACardCharacter *TargetCharacter)
{
	if (!InstigatorCharacter) return false;

	if (!SpendMana(InCardData.Cost))
	{
		return false;
	}

	UCardGameInstance *GameInst = Cast<UCardGameInstance>(GetGameInstance());

	switch (InCardData.TargetType)
	{
	case ECardTargetType::SingleEnemy:
		if (!TargetCharacter)
		{
			AddMana(InCardData.Cost);
			return false;
		}

		{
			int32 FinalDamage = InCardData.Damage;
			if (bNextCardDoubleDamage && FinalDamage > 0)
			{
				FinalDamage *= 2;
				bNextCardDoubleDamage = false;

				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("🔥 스킬 효과: 피해량 2배 발동!"));
			}

			TargetCharacter->TakeDamageCustom(FinalDamage);

			// ★ 스킬 객체에 데미지 입힘 이벤트 전달
			if (GameInst && GameInst->ActiveSkillInstance)
			{
				GameInst->ActiveSkillInstance->OnDamageDealt(FinalDamage);
			}
		}
		break;

	case ECardTargetType::Self:
		InstigatorCharacter->AddBlock(InCardData.Block);

		// ★ 스킬 객체에 방어도 획득 이벤트 전달
		if (GameInst && GameInst->ActiveSkillInstance)
		{
			GameInst->ActiveSkillInstance->OnBlockGained(InCardData.Block);
		}
		break;

	case ECardTargetType::AllEnemies:
		break;
	case ECardTargetType::None:
		break;
	}

	// ★ 구형 트리거 제거 완료. 새로운 스킬 객체에 카드 사용 이벤트 전달
	if (GameInst && GameInst->ActiveSkillInstance)
	{
		GameInst->ActiveSkillInstance->OnCardPlayed(InCardData.CardID);
	}

	DiscardCard(HandIndex);
	return true;
}

void UCardManager::EndPlayerTurn(ACardCharacter *PlayerChar, ACardCharacter *EnemyChar)
{
	// ★ 턴 종료 시 스킬 객체에 알림 (턴당 1회 제한 초기화, 턴 버프 해제 등)
	UCardGameInstance *GameInst = Cast<UCardGameInstance>(GetGameInstance());
	if (GameInst && GameInst->ActiveSkillInstance)
	{
		GameInst->ActiveSkillInstance->OnTurnEnded();
	}

	DiscardHand();

	if (EnemyChar && EnemyChar->CurrentHealth > 0 && PlayerChar)
	{
		if (AEnemyCharacter *ActualEnemy = Cast<AEnemyCharacter>(EnemyChar))
		{
			ActualEnemy->ResetForNewTurn();
			ActualEnemy->ExecuteIntent(PlayerChar);
			ActualEnemy->DecideNextIntent();
		}
	}

	if (PlayerChar)
	{
		PlayerChar->ResetForNewTurn();
	}

	ResetMana();
	DrawCards(5);
}

void UCardManager::ResetMana()
{
	UCardGameInstance *GameInst = Cast<UCardGameInstance>(GetGameInstance());
	if (GameInst)
	{
		CurrentMana = GameInst->CurrentPlayer.MaxMana;
		OnManaChanged.Broadcast(CurrentMana);
	}
}

void UCardManager::AddMana(const FManaGroup &InMana)
{
	CurrentMana.Red += InMana.Red;
	CurrentMana.Green += InMana.Green;
	CurrentMana.Blue += InMana.Blue;
	CurrentMana.White += InMana.White;
	CurrentMana.Black += InMana.Black;
	CurrentMana.Colorless += InMana.Colorless;

	OnManaChanged.Broadcast(CurrentMana);
}

bool UCardManager::CanPayMana(const FManaGroup &Cost) const
{
	if (CurrentMana.Red < Cost.Red ||
		CurrentMana.Green < Cost.Green ||
		CurrentMana.Blue < Cost.Blue ||
		CurrentMana.White < Cost.White ||
		CurrentMana.Black < Cost.Black)
	{
		return false;
	}

	int32 RemainingSurplus =
		(CurrentMana.Red - Cost.Red) +
		(CurrentMana.Green - Cost.Green) +
		(CurrentMana.Blue - Cost.Blue) +
		(CurrentMana.White - Cost.White) +
		(CurrentMana.Black - Cost.Black) +
		CurrentMana.Colorless;

	return RemainingSurplus >= Cost.Colorless;
}

bool UCardManager::SpendMana(const FManaGroup &Cost)
{
	if (!CanPayMana(Cost))
	{
		return false;
	}

	CurrentMana.Red -= Cost.Red;
	CurrentMana.Green -= Cost.Green;
	CurrentMana.Blue -= Cost.Blue;
	CurrentMana.White -= Cost.White;
	CurrentMana.Black -= Cost.Black;

	int32 GenericNeeded = Cost.Colorless;

	auto DeductGeneric = [&GenericNeeded](int32 &AvailableMana)
		{
			int32 Deduct = FMath::Min(AvailableMana, GenericNeeded);
			AvailableMana -= Deduct;
			GenericNeeded -= Deduct;
		};

	DeductGeneric(CurrentMana.Colorless);
	if (GenericNeeded > 0) DeductGeneric(CurrentMana.Red);
	if (GenericNeeded > 0) DeductGeneric(CurrentMana.Green);
	if (GenericNeeded > 0) DeductGeneric(CurrentMana.Blue);
	if (GenericNeeded > 0) DeductGeneric(CurrentMana.White);
	if (GenericNeeded > 0) DeductGeneric(CurrentMana.Black);

	UCardGameInstance *GameInst = Cast<UCardGameInstance>(GetGameInstance());
	if (GameInst && GameInst->ActiveSkillInstance)
	{
		int32 TotalManaSpent = Cost.Red + Cost.Green + Cost.Blue + Cost.White + Cost.Black + Cost.Colorless;
		if (TotalManaSpent > 0)
		{
			GameInst->ActiveSkillInstance->OnManaSpent(TotalManaSpent);
		}
	}

	OnManaChanged.Broadcast(CurrentMana);
	return true;
}