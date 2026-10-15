#pragma once

#include "CoreMinimal.h"
#include "ManaData.h"
#include "CardData.h"
#include "PlayerData.generated.h"

// 직업(클래스)을 정의하는 Enum 추가
UENUM(BlueprintType)
enum class EPlayerClass : uint8
{
	Knight UMETA(DisplayName = "기사"),
	Rogue  UMETA(DisplayName = "도적"),
	Mage   UMETA(DisplayName = "마법사")

	// 추후 추가
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	EPlayerClass PlayerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Mana")
	FManaGroup MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	TArray<FCardInstance> Deck;
};