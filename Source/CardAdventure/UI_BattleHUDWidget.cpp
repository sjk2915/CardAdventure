#include "UI_BattleHUDWidget.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "UI_ManaOrbWidget.h"

void UUI_BattleHUDWidget::UpdateCurrentMana(const FManaGroup &CurrentMana)
{
	if (!VB_CurrentManaContainer) return;

	// ★ ClearChildren() 대신, 0번(스페이서)만 남기고 나머지 구슬들을 지웁니다.
	while (VB_CurrentManaContainer->GetChildrenCount() > 1)
	{
		VB_CurrentManaContainer->RemoveChildAt(1);
	}

	// 위에서 아래로 스폰할 순서대로 호출
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.Red, EManaColor::Red);
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.Green, EManaColor::Green);
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.Blue, EManaColor::Blue);
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.White, EManaColor::White);
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.Black, EManaColor::Black);
	AddOrbsToVerticalBox(VB_CurrentManaContainer, CurrentMana.Colorless, EManaColor::Colorless);
}

void UUI_BattleHUDWidget::AddOrbsToVerticalBox(UVerticalBox *TargetBox, int32 Count, EManaColor ColorType)
{
	if (!TargetBox || !ManaOrbClass) return;

	for (int32 i = 0; i < Count; ++i)
	{
		UUI_ManaOrbWidget *NewOrb = CreateWidget<UUI_ManaOrbWidget>(GetWorld(), ManaOrbClass);
		if (NewOrb)
		{
			NewOrb->InitManaColor(ColorType);

			// ★ 0번은 스페이서 고정이므로, 무조건 '1번' 자리에 새 구슬을 끼워 넣습니다.
			// 이렇게 하면 나중에 들어온 구슬이 위를 차지하고, 먼저 들어간 구슬은 밑으로 밀려납니다.
			UVerticalBoxSlot *BoxSlot = Cast<UVerticalBoxSlot>(TargetBox->InsertChildAt(1, NewOrb));

			if (BoxSlot)
			{
				BoxSlot->SetHorizontalAlignment(HAlign_Center); // 가운데 정렬
				BoxSlot->SetVerticalAlignment(VAlign_Center);

				BoxSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f)); // 아래쪽 여백
			}
		}
	}
}