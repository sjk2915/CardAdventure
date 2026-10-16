#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CardData.h"
#include "StatusBase.h"
#include "CardCharacter.generated.h"

class UBoxComponent;
class UWidgetComponent;

// 체력 변경 시 호출될 델리게이트 선언 (현재 체력, 최대 체력 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, int32, NewCurrentHealth, int32, NewMaxHealth);

// 방어도 변경 시 호출될 델리게이트 선언 (최신 방어도 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockChangedSignature, int32, NewBlock);

// 사망 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDiedSignature, ACardCharacter *, DeadCharacter);

UCLASS()
class CARDADVENTURE_API ACardCharacter : public APawn
{
	GENERATED_BODY()

public:
	ACardCharacter();

protected:
	virtual void BeginPlay() override;

public:
	// --- 컴포넌트 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *BoxCollision;

	// ★ 체력바 위젯 컴포넌트 (모든 전투 캐릭터 공통)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|UI")
	TObjectPtr<UWidgetComponent> HPBarWidgetComp;

	// --- 전투 스테이터스 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHealth = 80;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 CurrentHealth = 80;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 Block = 0;

	// 체력 변경 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnHealthChangedSignature OnHealthChanged;

	// 방어도 변경 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnBlockChangedSignature OnBlockChanged;

	// ★ 객체화된 상태이상들을 담아둘 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Status")
	TArray<UStatusBase *> ActiveStatuses;

	// 상태이상 부여 함수 (C++ 및 블루프린트 공용)
	UFUNCTION(BlueprintCallable, Category = "Combat|Status")
	void AddStatus(TSubclassOf<UStatusBase> StatusClass, int32 Amount);

	// 상태이상 제거/스택 감소용 유틸리티
	UFUNCTION(BlueprintCallable, Category = "Combat|Status")
	void RemoveStatus(TSubclassOf<UStatusBase> StatusClass);

	// --- 전투 기능 함수 ---
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamageCustom(int32 DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void AddBlock(int32 BlockAmount);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ResetForNewTurn();

	// 사망 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Combat|Events")
	FOnCharacterDiedSignature OnCharacterDied;

	// 외부에서 체력을 덮어씌우는 함수 (전투 시작 시 플레이어 체력 동기화용)
	UFUNCTION(BlueprintCallable, Category = "Combat|Setup")
	void InitStats(int32 InMaxHP, int32 InCurrentHP);
};