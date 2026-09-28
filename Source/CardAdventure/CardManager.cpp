#include "CardManager.h"

UCardManager::UCardManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCardManager::BeginPlay()
{
	Super::BeginPlay();
}

// 1. 초기 덱 설정 및 셔플
void UCardManager::InitializeDeck(const TArray<FName> &StartingCardIDs)
{
	Deck = StartingCardIDs;
	DiscardPile.Empty();
	Hand.Empty();

	// 덱 셔플 (알고리즘 기반 무작위 섞기)
	for (int32 i = Deck.Num() - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
		Deck.Swap(i, RandomIndex);
	}

	UE_LOG(LogTemp, Warning, TEXT("[CardManager] Initialized Deck with %d cards."), Deck.Num());
}

// 2. 카드 뽑기
void UCardManager::DrawCards(int32 Amount)
{
	for (int32 i = 0; i < Amount; ++i)
	{
		// 덱이 비었으면 버린 카드 더미를 덱으로 섞음
		if (Deck.Num() == 0)
		{
			if (DiscardPile.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("[CardManager] No more cards to draw!"));
				break;
			}
			ShuffleDiscardIntoDeck();
		}

		// 덱의 맨 위(마지막 요소) 카드를 뽑아 손패로 이동
		FName DrawnCardID = Deck.Pop();
		Hand.Add(DrawnCardID);
		UE_LOG(LogTemp, Log, TEXT("[CardManager] Drew Card: %s (Hand Size: %d)"), *DrawnCardID.ToString(), Hand.Num());
	}
}

// 3. 버린 카드 더미 덱으로 셔플
void UCardManager::ShuffleDiscardIntoDeck()
{
	Deck = DiscardPile;
	DiscardPile.Empty();

	for (int32 i = Deck.Num() - 1; i > 0; --i)
	{
		int32 RandomIndex = FMath::RandRange(0, i);
		Deck.Swap(i, RandomIndex);
	}

	UE_LOG(LogTemp, Warning, TEXT("[CardManager] Shuffled Discard Pile into Deck. New Deck Count: %d"), Deck.Num());
}