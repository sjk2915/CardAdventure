#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardData.h"
#include "UI_HandWidget.generated.h"

class UHorizontalBox;
class UUI_CombatCardWidget; // ★ 변경됨: 이제 알맹이가 아니라 껍데기를 참조합니다.
class UDataTable;

UCLASS()
class CARDADVENTURE_API UUI_HandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 손패 갱신 함수 (파라미터는 기존과 동일하게 유지)
	UFUNCTION(BlueprintCallable, Category = "UI Hand")
	void RefreshHand(const TArray<FCardInstance> &HandCards, UDataTable *CardDataTable);

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox *Box_HandContainer;

	// ★ 변경됨: TSubclassOf 타입을 UUI_CombatCardWidget으로 교체
	UPROPERTY(EditDefaultsOnly, Category = "UI Hand")
	TSubclassOf<UUI_CombatCardWidget> CardWidgetClass;
};