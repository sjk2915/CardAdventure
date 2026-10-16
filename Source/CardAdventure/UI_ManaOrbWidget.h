#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManaData.h" // EManaColor 열거형 포함
#include "UI_ManaOrbWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class CARDADVENTURE_API UUI_ManaOrbWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 블루프린트의 구슬 이미지 컴포넌트
	UPROPERTY(meta = (BindWidget))
	UImage *Img_Orb;

	// --- 구슬 자신이 직접 텍스처를 가집니다 ---
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_Red;
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_Green;
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_Blue;
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_White;
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_Black;
	UPROPERTY(EditDefaultsOnly, Category = "Mana Texture")
	UTexture2D *Tex_Colorless;

public:
	// C++에서 호출할 초기화 함수
	void InitManaColor(EManaColor InColor);
};