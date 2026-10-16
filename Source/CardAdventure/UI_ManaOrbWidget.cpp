#include "UI_ManaOrbWidget.h"
#include "Components/Image.h"

void UUI_ManaOrbWidget::InitManaColor(EManaColor InColor)
{
	if (!Img_Orb) return;

	UTexture2D *SelectedTex = nullptr;

	// 넘겨받은 열거형(Enum)에 따라 스스로 텍스처를 결정
	switch (InColor)
	{
	case EManaColor::Red:       SelectedTex = Tex_Red; break;
	case EManaColor::Green:     SelectedTex = Tex_Green; break;
	case EManaColor::Blue:      SelectedTex = Tex_Blue; break;
	case EManaColor::White:     SelectedTex = Tex_White; break;
	case EManaColor::Black:     SelectedTex = Tex_Black; break;
	case EManaColor::Colorless: SelectedTex = Tex_Colorless; break;
	}

	if (SelectedTex)
	{
		Img_Orb->SetBrushFromTexture(SelectedTex);
	}
}