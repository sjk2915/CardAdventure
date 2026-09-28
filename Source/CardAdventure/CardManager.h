#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CardData.h"
#include "CardManager.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CARDADVENTURE_API UCardManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardManager();

protected:
	virtual void BeginPlay() override;

public:
	// 카드 데이터 테이블 에셋 연결 (에디터에서 지정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card System")
	UDataTable *CardDataTable;

	// 덱 (Draw Pile) - 카드 ID 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card System")
	TArray<FName> Deck;

	// 손패 (Hand) - 카드 ID 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card System")
	TArray<FName> Hand;

	// 버린 카드 더미 (Discard Pile) - 카드 ID 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card System")
	TArray<FName> DiscardPile;

	// 1. 초기 덱 구성 및 셔플
	UFUNCTION(BlueprintCallable, Category = "Card System")
	void InitializeDeck(const TArray<FName> &StartingCardIDs);

	// 2. 카드 뽑기 (Draw)
	UFUNCTION(BlueprintCallable, Category = "Card System")
	void DrawCards(int32 Amount);

	// 3. 버린 카드를 다시 덱으로 섞기
	UFUNCTION(BlueprintCallable, Category = "Card System")
	void ShuffleDiscardIntoDeck();
};