#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManaData.h" 
#include "UI_BattleHUDWidget.generated.h"

class UVerticalBox; // 세로 상자 사용!
class UUI_ManaOrbWidget;

UCLASS()
class CARDADVENTURE_API UUI_BattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 전투 HUD의 세로 상자 (블루프린트 변수명과 일치해야 함)
	UPROPERTY(meta = (BindWidget))
	UVerticalBox *VB_CurrentManaContainer;

	// 마나 구슬 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Mana UI")
	TSubclassOf<UUI_ManaOrbWidget> ManaOrbClass;

protected:
	// 세로 상자에 구슬을 생성하고 정렬하는 도우미 함수
	void AddOrbsToVerticalBox(UVerticalBox *TargetBox, int32 Count, EManaColor ColorType);

public:
	// 블루프린트의 OnManaChanged 이벤트에서 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateCurrentMana(const FManaGroup &CurrentMana);
};