#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ManaData.h"
#include "CardData.generated.h"

// 카드의 종류 (공격, 방어, 스킬, 파워 등)
UENUM(BlueprintType)
enum class ECardType : uint8
{
    Attack      UMETA(DisplayName = "Attack"),
    Defense     UMETA(DisplayName = "Defense"),
    Skill       UMETA(DisplayName = "Skill"),
    Power       UMETA(DisplayName = "Power")
};

// 카드의 타겟팅 대상 (어디에 드롭해야 하는가)
UENUM(BlueprintType)
enum class ECardTargetType : uint8
{
    SingleEnemy     UMETA(DisplayName = "Single Enemy"),
    AllEnemies      UMETA(DisplayName = "All Enemies"),
    Self            UMETA(DisplayName = "Self / Player"),
    None            UMETA(DisplayName = "No Target")
};

// 카드 스크린용 enum
UENUM(BlueprintType)
enum class ECardSelectMode : uint8
{
    View		UMETA(DisplayName = "보기 모드 (클릭 불가)"),
    Upgrade		UMETA(DisplayName = "강화 모드 (덮어쓰기)"),
    Remove		UMETA(DisplayName = "제거 모드 (삭제)"),
    Transform	UMETA(DisplayName = "변화 모드 (무작위 교체)"),
};

// 카드 데이터 테이블의 한 행(Row)을 정의하는 구조체
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FName CardID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FText CardName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    ECardType CardType = ECardType::Attack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    ECardTargetType TargetType = ECardTargetType::SingleEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FManaGroup Cost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    int32 Damage = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    int32 Block = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    TSoftObjectPtr<UTexture2D> CardImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data|Upgrade")
    FName TargetUpgradeID;
};

USTRUCT(BlueprintType)
struct FCardInstance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardInstance")
    FName CardID;
};