#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_CombatCardWidget.generated.h"

UCLASS()
class CARDADVENTURE_API UUI_CombatCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 이 위젯이 태어날 때 어떤 카드인지 식별하기 위한 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Card", meta = (ExposeOnSpawn = "true"))
	FName CardID;

	// 현재 내 카드가 손패에서 몇 번째에 위치하는지 저장하는 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat Card")
	int32 HandIndex = INDEX_NONE;

	// 손패 인덱스 설정
	UFUNCTION(BlueprintCallable, Category = "Combat Card")
	void SetHandIndex(int32 InIndex) { HandIndex = InIndex; }

	// 블루프린트의 OnDragCancelled 등에서 화면 좌표를 넘겨받아 카드 사용을 시도하는 C++ 함수
	UFUNCTION(BlueprintCallable, Category = "Combat Card|Action")
	void AttemptPlayCard(FVector2D ScreenPosition, ACardCharacter *PlayerCharacter, APlayerController *PC);
};