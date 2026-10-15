#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ShopWidget.generated.h"

class UWrapBox; // ⬅️ WrapBox 클래스 전방 선언
class UUI_ShopCardWidget;
class UCardManager;

UCLASS()
class CARDADVENTURE_API UUI_ShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 상점 카드 진열대 (카드가 끝에 닿으면 자동으로 다음 줄로 넘어가는 박스)
	UPROPERTY(meta = (BindWidget))
	UWrapBox *Wrap_CardContainer; // ⬅️ 변수명과 타입 변경

	// 생성할 상점 카드 위젯의 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UUI_ShopCardWidget> ShopCardClass;

	// 상점에 진열할 카드 개수 (이미지처럼 8장을 띄우려면 8로 세팅)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 ShopCardCount = 8;

public:
	UFUNCTION(BlueprintCallable, Category = "Shop")
	void PopulateShop();
};