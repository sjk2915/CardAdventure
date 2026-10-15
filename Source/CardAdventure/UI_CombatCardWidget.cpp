#include "UI_CombatCardWidget.h"
#include "Engine/Engine.h" // GEngine 사용을 위해 추가
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "CardManager.h"
#include "EnemyCharacter.h"

void UUI_CombatCardWidget::AttemptPlayCard(FVector2D ScreenPosition, ACardCharacter *PlayerCharacter, APlayerController *PC)
{
	if (!GEngine) return;

	if (!PC || !PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("[실패] PC 또는 PlayerCharacter가 비어있습니다. (블루프린트 핀 확인)"));
		SetVisibility(ESlateVisibility::Visible);
		return;
	}

	UGameInstance *GameInstance = GetGameInstance();
	UCardManager *CardManager = GameInstance ? GameInstance->GetSubsystem<UCardManager>() : nullptr;
	if (!CardManager || !CardManager->CardDataTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("[실패] CardManager 또는 DataTable을 찾을 수 없습니다."));
		SetVisibility(ESlateVisibility::Visible);
		return;
	}

	FCardData *CardDataPtr = CardManager->CardDataTable->FindRow<FCardData>(CardID, TEXT("PlayCardContext"));
	if (!CardDataPtr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("[실패] CardID [%s] 데이터를 찾을 수 없습니다."), *CardID.ToString()));
		SetVisibility(ESlateVisibility::Visible);
		return;
	}

	bool bCardPlayed = false;

	// === 타겟 타입에 따른 분기 ===
	if (CardDataPtr->TargetType == ECardTargetType::SingleEnemy)
	{
		FVector WorldLocation, WorldDirection;
		if (PC->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
		{
			FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(PlayerCharacter);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
			{
				AEnemyCharacter *TargetEnemy = Cast<AEnemyCharacter>(HitResult.GetActor());
				if (TargetEnemy)
				{
					bCardPlayed = CardManager->ExecuteCard(*CardDataPtr, HandIndex, PlayerCharacter, TargetEnemy);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("적 타겟팅 성공! ExecuteCard 실행 결과: %d"), bCardPlayed));
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("[실패] 맞은 액터가 적이 아님: %s"), *HitResult.GetActor()->GetName()));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("[실패] 허공에 드롭했습니다. (레이캐스트 안 맞음)"));
			}
		}
	}
	else
	{
		int32 ViewportSizeX, ViewportSizeY;
		PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

		if (ScreenPosition.Y < (ViewportSizeY * 0.75f))
		{
			bCardPlayed = CardManager->ExecuteCard(*CardDataPtr, HandIndex, PlayerCharacter, PlayerCharacter);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("수비/광역기 실행 성공! ExecuteCard 결과: %d"), bCardPlayed));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("[실패] Y축이 화면 75%% 아래입니다. 현재 Y: %f, 기준점: %f"), ScreenPosition.Y, ViewportSizeY * 0.75f));
		}
	}

	if (bCardPlayed)
	{
		RemoveFromParent();
	}
	else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}