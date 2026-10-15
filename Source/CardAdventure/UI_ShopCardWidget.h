#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ShopCardWidget.generated.h"

UCLASS()
class CARDADVENTURE_API UUI_ShopCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이 카드가 어떤 카드인지 식별
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop Card", meta = (ExposeOnSpawn = "true"))
	FName CardID;

	// 카드의 가격
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop Card")
	int32 Price;

	// 카드의 원래 가격 (할인 이벤트 등을 대비해 원가를 저장해두면 좋습니다)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shop Card")
	int32 OriginalPrice;

	// 버튼 클릭 시 구매를 시도하는 함수
	UFUNCTION(BlueprintCallable, Category = "Shop Card|Action")
	void AttemptBuyCard();
};