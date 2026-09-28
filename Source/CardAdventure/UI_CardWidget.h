#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "UI_CardWidget.generated.h"

// 전방 선언 (가볍게 텍스트/이미지 클래스 참조)
class UTextBlock;
class UImage;

UCLASS()
class CARDADVENTURE_API UUI_CardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 카드 데이터를 바탕으로 UI 비주얼 업데이트
	UFUNCTION(BlueprintCallable, Category = "UI Card")
	void SetCardData(const FCardData &InCardData);

protected:
	// UMG 위젯들과 C++ 변수를 자동으로 1:1 바인딩
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_CardName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Cost;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_CardType;

	UPROPERTY(meta = (BindWidget))
	UImage *Image_CardArt;
};