#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	// 머리 위 위젯 컴포넌트 생성 및 루트 하위 부착
	IntentWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("IntentWidgetComp"));
	IntentWidgetComp->SetupAttachment(RootComponent);

	// 턴제 게임 인게임 UI 기본 세팅 (스크린 스페이스 권장)
	IntentWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	IntentWidgetComp->SetDrawAtDesiredSize(true);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 전투 스폰 직후 첫 번째 의도 자동 결정
	DecideNextIntent();
}

void AEnemyCharacter::DecideNextIntent()
{
	// 패턴 배열이 비어있는 경우 대비 (예외 방지용 기본 공격)
	if (ActionPattern.Num() == 0)
	{
		CurrentIntent.IntentType = EEnemyIntentType::Attack;
		CurrentIntent.Value = 6;
		CurrentIntent.HitCount = 1;
		OnIntentChanged.Broadcast(CurrentIntent);
		return;
	}

	// 1. 다음 패턴 인덱스로 이동 (배열 끝에 도달하면 0번으로 루프)
	CurrentPatternIndex = (CurrentPatternIndex + 1) % ActionPattern.Num();

	// 2. 해당 순번의 의도 로드
	CurrentIntent = ActionPattern[CurrentPatternIndex];

	// 3. 머리 위 UI에 브로드캐스트
	OnIntentChanged.Broadcast(CurrentIntent);
}

void AEnemyCharacter::ExecuteIntent(ACardCharacter *TargetPlayer)
{
	if (CurrentHealth <= 0) return;

	switch (CurrentIntent.IntentType)
	{
	case EEnemyIntentType::Attack:
		if (TargetPlayer)
		{
			for (int32 i = 0; i < CurrentIntent.HitCount; ++i)
			{
				TargetPlayer->TakeDamageCustom(CurrentIntent.Value);
			}
		}
		break;

	case EEnemyIntentType::Defend:
		AddBlock(CurrentIntent.Value);
		break;

	case EEnemyIntentType::Buff:
	case EEnemyIntentType::Debuff:
		// 추후 확장 지점
		break;

	default:
		break;
	}
}