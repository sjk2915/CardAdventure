#include "CardCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

ACardCharacter::ACardCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. 박스 콜리전 생성 및 루트 컴포넌트 등록
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
	BoxCollision->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	BoxCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 2. 체력바 위젯 컴포넌트 생성 및 루트 부착
	HPBarWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComp"));
	HPBarWidgetComp->SetupAttachment(RootComponent);
	HPBarWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidgetComp->SetDrawAtDesiredSize(true);
	HPBarWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
}

void ACardCharacter::BeginPlay()
{
	Super::BeginPlay();
	ResetForNewTurn();
}

void ACardCharacter::InitStats(int32 InMaxHP, int32 InCurrentHP)
{
	MaxHealth = InMaxHP;
	CurrentHealth = InCurrentHP;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ACardCharacter::AddStatus(TSubclassOf<UStatusBase> StatusClass, int32 Amount)
{
	if (!StatusClass || Amount <= 0) return;

	// 1. 이미 같은 버프가 있는지 확인 (있으면 스택만 증가)
	for (UStatusBase *ExistingStatus : ActiveStatuses)
	{
		if (ExistingStatus->IsA(StatusClass))
		{
			ExistingStatus->StackCount += Amount;
			return; // 추가했으니 끝!
		}
	}

	// 2. 없다면 새로 붕어빵을 구워서 배열에 넣음
	UStatusBase *NewStatus = NewObject<UStatusBase>(this, StatusClass);
	NewStatus->OwnerCharacter = this;
	NewStatus->StackCount = Amount;
	ActiveStatuses.Add(NewStatus);
}

void ACardCharacter::RemoveStatus(TSubclassOf<UStatusBase> StatusClass)
{
	// 배열을 뒤에서부터 돌면서 해당 클래스와 일치하는 버프를 삭제 (안전한 배열 삭제 방법)
	for (int32 i = ActiveStatuses.Num() - 1; i >= 0; --i)
	{
		if (ActiveStatuses[i]->IsA(StatusClass))
		{
			ActiveStatuses.RemoveAt(i);
			break;
		}
	}
}

void ACardCharacter::TakeDamageCustom(int32 DamageAmount)
{
	if (DamageAmount <= 0) return;

	// 방어도 우선 차감 로직
	int32 RemainingDamage = DamageAmount;
	if (Block > 0)
	{
		if (Block >= RemainingDamage)
		{
			Block -= RemainingDamage;
			RemainingDamage = 0;
		}
		else
		{
			RemainingDamage -= Block;
			Block = 0;
		}

		// 방어도 변동 알림
		OnBlockChanged.Broadcast(Block);
	}

	// 남은 데미지 체력 차감
	if (RemainingDamage > 0)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - RemainingDamage, 0, MaxHealth);
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	}

	if (CurrentHealth <= 0)
	{
		// ★ 사망 로직 추가: 누군가 죽었다고 이벤트 발송!
		OnCharacterDied.Broadcast(this);
	}
}

void ACardCharacter::AddBlock(int32 Amount)
{
	Block += Amount;
	OnBlockChanged.Broadcast(Block);
}

void ACardCharacter::ResetForNewTurn()
{
	// 방어도 리셋 및 알림 (마나 리셋은 CardManager로 이동됨)
	Block = 0;
	OnBlockChanged.Broadcast(Block);
}