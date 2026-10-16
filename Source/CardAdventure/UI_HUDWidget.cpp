#include "UI_HUDWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUI_HUDWidget::UpdateHUD(const FPlayerData &PlayerData)
{
	// 1. 체력 텍스트 업데이트
	if (Txt_Health)
	{
		FString HPString = FString::Printf(TEXT("HP : %d / %d"), PlayerData.CurrentHP, PlayerData.MaxHP);
		Txt_Health->SetText(FText::FromString(HPString));
	}

	// 2. 골드 텍스트 업데이트
	if (Txt_Gold)
	{
		FString GoldString = FString::Printf(TEXT("%dG"), PlayerData.Gold);
		Txt_Gold->SetText(FText::FromString(GoldString));
	}

	// 3. 직업에 따른 캐릭터 초상화 이미지 분기 처리
	if (Img_Character)
	{
		UTexture2D *SelectedTex = nullptr;

		switch (PlayerData.PlayerClass)
		{
		case EPlayerClass::Knight: SelectedTex = Tex_Knight; break;
		case EPlayerClass::Rogue:  SelectedTex = Tex_Rogue;  break;
		case EPlayerClass::Mage:   SelectedTex = Tex_Mage;   break;
		}

		if (SelectedTex)
		{
			Img_Character->SetBrushFromTexture(SelectedTex);
		}
	}

	// 4. 최대 마나 그리기 (이제 텍스처 포인터가 아니라 Enum을 던집니다)
	if (HB_MaxManaContainer)
	{
		HB_MaxManaContainer->ClearChildren();
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.Red, EManaColor::Red);
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.Green, EManaColor::Green);
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.Blue, EManaColor::Blue);
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.White, EManaColor::White);
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.Black, EManaColor::Black);
		AddOrbsToBox(HB_MaxManaContainer, PlayerData.MaxMana.Colorless, EManaColor::Colorless);
	}
}

void UUI_HUDWidget::AddOrbsToBox(UHorizontalBox *TargetBox, int32 Count, EManaColor ColorType)
{
	if (!TargetBox || !ManaOrbClass) return;

	for (int32 i = 0; i < Count; ++i)
	{
		UUI_ManaOrbWidget *NewOrb = CreateWidget<UUI_ManaOrbWidget>(GetWorld(), ManaOrbClass);
		if (NewOrb)
		{
			NewOrb->InitManaColor(ColorType);

			// ★ 위젯을 가로 상자에 추가하고 '슬롯(Slot)' 정보를 받아옵니다.
			UHorizontalBoxSlot *BoxSlot = TargetBox->AddChildToHorizontalBox(NewOrb);

			if (BoxSlot)
			{
				BoxSlot->SetVerticalAlignment(VAlign_Center);
				BoxSlot->SetHorizontalAlignment(HAlign_Center);

				// 3. (보너스) 구슬끼리 다닥다닥 붙지 않게 우측에 5px 여백(Padding) 추가
				BoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
			}
		}
	}
}