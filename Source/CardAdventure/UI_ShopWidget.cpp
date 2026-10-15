#include "UI_ShopWidget.h"
#include "UI_ShopCardWidget.h"
#include "Components/WrapBox.h" // ⬅️ 헤더 변경
#include "Engine/GameInstance.h"
#include "CardManager.h"
#include "Components/WrapBoxSlot.h"

void UUI_ShopWidget::PopulateShop()
{
	if (!Wrap_CardContainer || !ShopCardClass) return; // ⬅️ 이름 변경

	// 1. 기존 진열 비우기
	Wrap_CardContainer->ClearChildren(); // ⬅️ 이름 변경

	UGameInstance *GameInstance = GetGameInstance();
	if (!GameInstance) return;

	UCardManager *CardManager = GameInstance->GetSubsystem<UCardManager>();
	if (!CardManager || !CardManager->CardDataTable) return;

	TArray<FName> AllCardIDs = CardManager->CardDataTable->GetRowNames();
	if (AllCardIDs.Num() == 0) return;

	for (int32 i = 0; i < ShopCardCount; ++i)
	{
		int32 RandomIndex = FMath::RandRange(0, AllCardIDs.Num() - 1);
		FName RandomCardID = AllCardIDs[RandomIndex];

		FCardData *CardData = CardManager->CardDataTable->FindRow<FCardData>(RandomCardID, TEXT("ShopGeneration"));
		if (!CardData) continue;

		UUI_ShopCardWidget *NewShopCard = CreateWidget<UUI_ShopCardWidget>(this, ShopCardClass);
		if (NewShopCard)
		{
			NewShopCard->CardID = RandomCardID;

			int32 BasePrice = 50;
			float RandomMultiplier = FMath::RandRange(0.9f, 1.1f);
			NewShopCard->Price = FMath::RoundToInt(BasePrice * RandomMultiplier);
			NewShopCard->OriginalPrice = BasePrice;

			// 6. 완성된 카드를 랩 박스에 올리고 슬롯 정보 가져오기
			UWrapBoxSlot *WrapSlot = Cast<UWrapBoxSlot>(Wrap_CardContainer->AddChildToWrapBox(NewShopCard));
			if (WrapSlot)
			{
				// 블루프린트의 Set Horizontal Alignment (Center)와 동일한 역할
				WrapSlot->SetHorizontalAlignment(HAlign_Center);
				WrapSlot->SetVerticalAlignment(VAlign_Center);

				// 카드 사이의 여백(Padding) 설정 (좌, 상, 우, 하)
				WrapSlot->SetPadding(FMargin(10.0f, 10.0f, 10.0f, 10.0f));
				WrapSlot->SetFillEmptySpace(true);
			}
		}
	}
}