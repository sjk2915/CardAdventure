#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CardData.generated.h"

// 카드의 종류 (공격, 방어, 스킬 등)
UENUM(BlueprintType)
enum class ECardType : uint8
{
    Attack      UMETA(DisplayName = "Attack"),
    Defense     UMETA(DisplayName = "Defense"),
    Skill       UMETA(DisplayName = "Skill"),
    Power       UMETA(DisplayName = "Power")
};

// 카드 데이터 테이블의 한 행(Row)을 정의하는 구조체
USTRUCT(BlueprintType)
struct FCardData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 카드 ID (예: "C001")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FName CardID;

    // 카드 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FText CardName;

    // 카드 코스트 (마나)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    int32 Cost = 1;

    // 카드 유형
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    ECardType CardType = ECardType::Attack;

    // 데미지 수치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    int32 Damage = 0;

    // 방어도 수치
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    int32 Block = 0;

    // 카드 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    FText Description;

    // 카드 일러스트 텍스처 (2D)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Data")
    TSoftObjectPtr<UTexture2D> CardImage;
};