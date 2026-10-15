#include "UI_CardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UUI_CardWidget::UpdateCardVisual(const FCardData &InCardData)
{
	// 멤버 변수(CardData = InCardData;)로 저장하는 로직 삭제. 
	// 즉시 UI 갱신에만 사용합니다.

	if (Text_CardName)
	{
		Text_CardName->SetText(InCardData.CardName);
	}

	if (Text_Cost)
	{
		Text_Cost->SetText(FText::AsNumber(InCardData.Cost.GetTotal()));
	}

	if (Text_Description)
	{
		Text_Description->SetText(InCardData.Description);
	}

	if (Text_CardType)
	{
		Text_CardType->SetText(UEnum::GetDisplayValueAsText(InCardData.CardType));
	}

	if (Image_CardArt)
	{
		if (InCardData.CardImage.IsValid())
		{
			Image_CardArt->SetBrushFromTexture(InCardData.CardImage.Get());
		}
		else if (!InCardData.CardImage.IsNull())
		{
			UTexture2D *LoadedTexture = InCardData.CardImage.LoadSynchronous();
			if (LoadedTexture)
			{
				Image_CardArt->SetBrushFromTexture(LoadedTexture);
			}
		}
	}
}