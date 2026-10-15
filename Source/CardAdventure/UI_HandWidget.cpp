#include "UI_HandWidget.h"
#include "UI_CombatCardWidget.h" // ★ 껍데기 헤더 포함
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/DataTable.h"

void UUI_HandWidget::RefreshHand(const TArray<FCardInstance> &HandCards, UDataTable *CardDataTable)
{
	if (!Box_HandContainer || !CardWidgetClass)
	{
		return;
	}

	// 기존 손패 UI 비우기
	Box_HandContainer->ClearChildren();

	// 인덱스를 추적하며 손패 위젯 생성
	for (int32 i = 0; i < HandCards.Num(); ++i)
	{
		const FName &CardID = HandCards[i].CardID;

		// (선택 사항) 데이터 테이블에 실제로 존재하는 카드인지 유효성 검사만 수행
		if (CardDataTable)
		{
			FCardData *CardData = CardDataTable->FindRow<FCardData>(CardID, TEXT("UI_HandWidget_Refresh"));
			if (!CardData) continue; // 없는 카드면 스킵
		}

		// ★ 알맹이 대신 껍데기(UI_CombatCardWidget)를 생성합니다.
		UUI_CombatCardWidget *NewCardWidget = CreateWidget<UUI_CombatCardWidget>(this, CardWidgetClass);
		if (NewCardWidget)
		{
			// ★ 이제 그림(FCardData)을 넘겨주지 않습니다!
			// 껍데기에게 필수 전투 정보(CardID, HandIndex)만 주입합니다.
			NewCardWidget->CardID = CardID;
			NewCardWidget->SetHandIndex(i);

			// 박스에 추가
			UHorizontalBoxSlot *NewSlot = Box_HandContainer->AddChildToHorizontalBox(NewCardWidget);
			if (NewSlot)
			{
				// 카드 간격 패딩 설정 (좌우 5px 여백)
				NewSlot->SetPadding(FMargin(5.0f, 0.0f, 5.0f, 0.0f));
			}
		}
	}
}