#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerData.h" 
#include "UI_HUDWidget.generated.h"

class UHorizontalBox;
class UImage;
class UTextBlock;
class UTexture2D;
class UUI_ManaOrbWidget; // 전방 선언 추가

UCLASS()
class CARDADVENTURE_API UUI_HUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage *Img_Character;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *Txt_Health;

	UPROPERTY(meta = (BindWidget))
	UTextBlock *Txt_Gold;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox *HB_MaxManaContainer;

	// 직업 초상화
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	UTexture2D *Tex_Knight;
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	UTexture2D *Tex_Rogue;
	UPROPERTY(EditDefaultsOnly, Category = "Player UI")
	UTexture2D *Tex_Mage;

	// ★ 수정: UUserWidget이 아니라 우리가 방금 만든 UUI_ManaOrbWidget 클래스 타입으로 받습니다!
	UPROPERTY(EditDefaultsOnly, Category = "Mana UI")
	TSubclassOf<UUI_ManaOrbWidget> ManaOrbClass;

	// (기존에 있던 Tex_Red 등 6개의 텍스처와 OnManaOrbSpawned 이벤트는 모두 지웠습니다)

protected:
	// 텍스처 대신 EManaColor만 넘겨줍니다.
	void AddOrbsToBox(UHorizontalBox *TargetBox, int32 Count, EManaColor ColorType);

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHUD(const FPlayerData &PlayerData);
};