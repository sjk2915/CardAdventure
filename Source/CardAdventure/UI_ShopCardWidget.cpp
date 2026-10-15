#include "UI_ShopCardWidget.h" // 이름 수정 시 헤더명도 맞춰주세요!
#include "CardGameInstance.h"
#include "Engine/Engine.h"

void UUI_ShopCardWidget::AttemptBuyCard()
{
	// 1. 게임 인스턴스 가져오기
	UCardGameInstance *GameInstance = Cast<UCardGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	// 2. 플레이어의 골드가 가격보다 많은지(살 수 있는지) 확인
	if (GameInstance->CurrentPlayer.Gold >= Price)
	{
		// 골드 차감 (유저님의 AddGold 함수에 음수를 넣으면 자연스럽게 차감됩니다)
		GameInstance->AddGold(-Price);

		// 덱에 카드 추가 (유저님이 만들어두신 함수 활용)
		GameInstance->AddCardToDeck(CardID);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("[%s] 구매 성공! 남은 골드: %d"), *CardID.ToString(), GameInstance->CurrentPlayer.Gold));
		}

		// 카드가 팔렸으니 진열대(화면)에서 치워줍니다.
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		// 골드 부족
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("골드가 부족합니다!"));
		}
	}
}