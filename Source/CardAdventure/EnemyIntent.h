#pragma once

#include "CoreMinimal.h"
#include "EnemyIntent.generated.h"

// 적 의도 분류
UENUM(BlueprintType)
enum class EEnemyIntentType : uint8
{
	Attack    UMETA(DisplayName = "Attack (공격)"),
	Defend    UMETA(DisplayName = "Defend (방어)"),
	Buff      UMETA(DisplayName = "Buff (버프)"),
	Debuff    UMETA(DisplayName = "Debuff (디버프)")
};

// 적 의도 데이터 구조체
USTRUCT(BlueprintType)
struct FEnemyIntent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intent")
	EEnemyIntentType IntentType = EEnemyIntentType::Attack;

	// 공격력 또는 방어도 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intent")
	int32 Value = 0;

	// 공격 횟수 (1이면 단타, 2 이상이면 연타)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Intent")
	int32 HitCount = 1;
};