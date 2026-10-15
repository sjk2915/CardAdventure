#pragma once

#include "CoreMinimal.h"
#include "ManaData.generated.h"

// 1. 마나 색상 열거형
UENUM(BlueprintType)
enum class EManaColor : uint8
{
    Red       UMETA(DisplayName = "Red (적)"),
    Green     UMETA(DisplayName = "Green (녹)"),
    Blue      UMETA(DisplayName = "Blue (청)"),
    White     UMETA(DisplayName = "White (백)"),
    Black     UMETA(DisplayName = "Black (흑)"),
    Colorless UMETA(DisplayName = "Colorless (무색/아무거나)")
};

// 2. 마나 풀 및 코스트를 다루는 공용 구조체
USTRUCT(BlueprintType)
struct FManaGroup
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 Red = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 Green = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 Blue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 White = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 Black = 0;

    // 카드 코스트에서 '아무 색상 마나나 사용 가능한 수치'
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
    int32 Colorless = 0;

    // 총 마나 수 합계 계산
    int32 GetTotal() const
    {
        return Red + Green + Blue + White + Black + Colorless;
    }

    // 모든 마나 0으로 초기화
    void Clear()
    {
        Red = Green = Blue = White = Black = Colorless = 0;
    }
};