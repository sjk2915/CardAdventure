#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusBase.generated.h"

class ACardCharacter;

UCLASS(Blueprintable, BlueprintType)
class CARDADVENTURE_API UStatusBase : public UObject
{
	GENERATED_BODY()

public:
	// 상태이상 기본 정보 (블루프린트 디테일 패널에서 기획자가 세팅)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Info")
	FName StatusName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Info")
	bool bIsDebuff;

	// 현재 스택 수 (지속 턴 수 or 고정 수치)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status State")
	int32 StackCount;

	// 이 버프를 가지고 있는 주인 (나중에 데미지 줄 때 등 활용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status State")
	ACardCharacter *OwnerCharacter;

	// ----------------------------------------------------
	// ⚡ 블루프린트에서 구현할 이벤트들 (BlueprintNativeEvent)
	// ----------------------------------------------------

	// 공격할 때 데미지 공식을 조작할 기회를 줌
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Event")
	int32 OnModifyDamageDealt(int32 BaseDamage);
	virtual int32 OnModifyDamageDealt_Implementation(int32 BaseDamage) { return BaseDamage; } // 기본값: 그대로 반환

	// 맞을 때 데미지 공식을 조작할 기회를 줌
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Event")
	int32 OnModifyDamageTaken(int32 BaseDamage);
	virtual int32 OnModifyDamageTaken_Implementation(int32 BaseDamage) { return BaseDamage; } // 기본값: 그대로 반환

	// 턴 시작 / 종료 시 발동 (중독 데미지, 버프 스택 감소 등)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Event")
	void OnTurnStarted();
	virtual void OnTurnStarted_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Status Event")
	void OnTurnEnded();
	virtual void OnTurnEnded_Implementation() {}
};