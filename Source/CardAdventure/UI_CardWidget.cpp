#include "UI_CardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUI_CardWidget::SetCardData(const FCardData &InCardData)
{
	// 카드 이름 설정
	if (Text_CardName)
	{
		Text_CardName->SetText(InCardData.CardName);
	}

	// 코스트(마나) 설정 - 추후 색상 마나 확장 시 텍스트 스타일/색상 변경 가능
	if (Text_Cost)
	{
		Text_Cost->SetText(FText::AsNumber(InCardData.Cost));
	}

	// 설명문 설정
	if (Text_Description)
	{
		Text_Description->SetText(InCardData.Description);
	}

	// 카드 유형 텍스트 설정
	if (Text_CardType)
	{
		FString TypeStr = UEnum::GetDisplayValueAsText(InCardData.CardType).ToString();
		Text_CardType->SetText(FText::FromString(TypeStr));
	}

	// 카드 일러스트 이미지 설정 (소프트 레퍼런스 비동기 로드 대응 준비)
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