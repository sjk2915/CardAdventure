#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

class UCardManager;

// 블루프린트에서 상속받아 시각적으로 노드를 짤 수 있도록 허용
UCLASS(Blueprintable, BlueprintType)
class CARDADVENTURE_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	// 블루프린트에서 World Context 핀이 뜨지 않도록 월드 정보를 제공
	virtual UWorld *GetWorld() const override;

	// --- 스킬 기본 정보 ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Info")
	FName SkillName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Info")
	FString SkillDescription;

	// --- 런타임 스킬 상태 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill State")
	bool bIsReady = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill State")
	bool bUsedThisTurn = false;

	// --- ⚡ 블루프린트에서 구현할 이벤트 노드들 ---

	// 1. 스킬을 사용했을 때 (실제 효과 발동 로직을 블루프린트에 작성)
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Skill Event")
	void OnSkillExecuted(UCardManager *CardManager);

	// 2. 카드를 냈을 때
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Event")
	void OnCardPlayed(FName CardID);

	// 3. 적에게 피해를 입혔을 때
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Event")
	void OnDamageDealt(int32 DamageAmount);

	// 4. 방어도를 얻었을 때
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Event")
	void OnBlockGained(int32 BlockAmount);

	// 5. 마나를 소모했을 때
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Event")
	void OnManaSpent(int32 ManaAmount);

	// 6. 플레이어 턴이 끝났을 때 (턴당 1회 제한 초기화 등)
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Event")
	void OnTurnEnded();

	// 전투 시작 시 호출 (스택 초기화 및 UI 리셋용)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Event")
	void OnBattleStarted();
	virtual void OnBattleStarted_Implementation() {}
};