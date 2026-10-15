#pragma once

#include "CoreMinimal.h"
#include "CardCharacter.h"
#include "EnemyIntent.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

// 적 의도 갱신 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntentChangedSignature, const FEnemyIntent &, NewIntent);

UCLASS()
class CARDADVENTURE_API AEnemyCharacter : public ACardCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	// 적 의도 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> IntentWidgetComp;

public:
	UPROPERTY(BlueprintAssignable, Category = "Enemy|Events")
	FOnIntentChangedSignature OnIntentChanged;

	// 현재 턴에 준비된 의도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Intent")
	FEnemyIntent CurrentIntent;

	// ★ 에디터에서 정해줄 고정 행동 순환 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Intent")
	TArray<FEnemyIntent> ActionPattern;

	// 현재 진행 중인 패턴 인덱스 (기본 -1: 첫 턴에 0번으로 시작하도록)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Intent")
	int32 CurrentPatternIndex = -1;

	// 다음 턴에 행할 의도 결정 (배열 순차 순환)
	UFUNCTION(BlueprintCallable, Category = "Enemy|Intent")
	virtual void DecideNextIntent();

	// 결정된 의도 실행
	UFUNCTION(BlueprintCallable, Category = "Enemy|Intent")
	virtual void ExecuteIntent(ACardCharacter *TargetPlayer);
};