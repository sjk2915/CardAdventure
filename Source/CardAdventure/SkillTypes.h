#pragma once

#include "CoreMinimal.h"
#include "SkillTypes.generated.h"

// 1. 스킬 충전 조건 (Trigger)
UENUM(BlueprintType)
enum class ESkillChargeTrigger : uint8
{
	OnCardPlayed    UMETA(DisplayName = "카드 사용 시 1 충전"),
	OnEnergySpent   UMETA(DisplayName = "에너지 1 소모 당 1 충전"),
	OnDamageDealt   UMETA(DisplayName = "피해 1을 입힐 때마다 1 충전")
};

// 2. 스킬 사용 효과 (Effect)
UENUM(BlueprintType)
enum class ESkillEffectType : uint8
{
	NextCardDoubleDamage  UMETA(DisplayName = "다음 카드 피해 2배"),
	DrawCards             UMETA(DisplayName = "카드 드로우"),
	GainEnergy            UMETA(DisplayName = "에너지 획득")
};

// 3. 스킬 데이터 구조체
USTRUCT(BlueprintType)
struct FPlayerSkill
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillChargeTrigger ChargeTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 MaxCharge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 CurrentCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	ESkillEffectType EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 EffectValue;

	// 기본 생성자 (기본값 세팅)
	FPlayerSkill()
		: SkillName(TEXT("None")), ChargeTrigger(ESkillChargeTrigger::OnCardPlayed),
		MaxCharge(5), CurrentCharge(0), EffectType(ESkillEffectType::DrawCards), EffectValue(1)
	{}
};