#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "CardData.h"
#include "CardManager.generated.h"

class ACardCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandUpdatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeckCountsUpdatedSignature, int32, DrawPileCount, int32, DiscardPileCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, FManaGroup, NewMana);

UCLASS()
class CARDADVENTURE_API UCardManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;

	// --- 델리게이트 ---
	UPROPERTY(BlueprintAssignable, Category = "Card|Events")
	FOnHandUpdatedSignature OnHandUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Card|Events")
	FOnDeckCountsUpdatedSignature OnDeckCountsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Mana|Events")
	FOnManaChangedSignature OnManaChanged;

	void UpdateDeckCounts()
	{
		OnDeckCountsUpdated.Broadcast(DrawPile.Num(), DiscardPile.Num());
	}

	// --- 덱 데이터 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card|Deck")
	TArray<FCardInstance> Deck;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card|Battle")
	TArray<FCardInstance> DrawPile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card|Battle")
	TArray<FCardInstance> Hand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card|Battle")
	TArray<FCardInstance> DiscardPile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card|Data")
	UDataTable *CardDataTable;

	// --- 카드 시스템 ---
	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	void StartBattle();

	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	void DrawCards(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	void DiscardCard(int32 HandIndex);

	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	void DiscardHand();

	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	void ShuffleDiscardIntoDraw();

	UFUNCTION(BlueprintCallable, Category = "Card|Battle")
	bool ExecuteCard(const FCardData &InCardData, int32 HandIndex, ACardCharacter *InstigatorCharacter, ACardCharacter *TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Card|Turn")
	void EndPlayerTurn(ACardCharacter *PlayerChar, ACardCharacter *EnemyChar);

	// ===================================================
	// ⚔️ 전투 및 상태 효과 시스템 (Combat & Status)
	// ===================================================

	// ★ 데미지 계산 (버프/디버프 적용)
	UFUNCTION(BlueprintPure, Category = "Combat|Damage")
	int32 CalculateDamage(int32 BaseDamage, ACardCharacter *Attacker, ACardCharacter *Defender);

	// ===================================================

	// --- 마나 데이터 및 제어 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	FManaGroup CurrentMana;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void ResetMana();

	UFUNCTION(BlueprintPure, Category = "Mana")
	bool CanPayMana(const FManaGroup &Cost) const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	bool SpendMana(const FManaGroup &Cost);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void AddMana(const FManaGroup &InMana);
};