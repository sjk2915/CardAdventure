#pragma once

#include "CoreMinimal.h"
#include "StatusData.generated.h"

UENUM(BlueprintType)
enum class EStatusEffect : uint8
{
	None UMETA(Hidden),

	// === 디버프 (Debuffs) ===
	Vulnerable UMETA(DisplayName = "취약"),
	Weak UMETA(DisplayName = "약화"),
	Frail UMETA(DisplayName = "분쇄"),
	Shackles UMETA(DisplayName = "쇠약"),
	Poison UMETA(DisplayName = "중독"),
	Burn UMETA(DisplayName = "화상"),
	Freeze UMETA(DisplayName = "빙결"),

	// === 버프 (Buffs) ===
	Strength UMETA(DisplayName = "힘"),
	Dexterity UMETA(DisplayName = "민첩"),
	Dodge UMETA(DisplayName = "회피"),
	Artifact UMETA(DisplayName = "인공물"),
	Thorns UMETA(DisplayName = "가시"),
	Barricade UMETA(DisplayName = "철벽")
};