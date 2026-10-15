#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "UI_CardWidget.generated.h"

class UTextBlock;
class UImage;

// 오직 카드의 시각적 요소(일러스트, 텍스트)만 렌더링하는 순수 뷰(View) 클래스
UCLASS()
class CARDADVENTURE_API UUI_CardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 카드 데이터를 주입받아 UI 위젯 요소를 갱신만 하는 함수 (상태 저장 안 함)
	UFUNCTION(BlueprintCallable, Category = "UI Card Visual")
	void UpdateCardVisual(const FCardData &InCardData);

protected:
	// --- UMG UI 바인딩 요소 (시각 요소만 남김) ---
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI Card Visual|Components")
	TObjectPtr<UTextBlock> Text_CardName;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI Card Visual|Components")
	TObjectPtr<UTextBlock> Text_Cost;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI Card Visual|Components")
	TObjectPtr<UTextBlock> Text_Description;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI Card Visual|Components")
	TObjectPtr<UTextBlock> Text_CardType;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "UI Card Visual|Components")
	TObjectPtr<UImage> Image_CardArt;
};