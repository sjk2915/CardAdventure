#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MapData.h"
#include "PlayerData.h"
#include "EventData.h"
#include "SkillBase.h" // ★ 기존 SkillTypes.h 대신 SkillBase.h 포함
#include "CardGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerStatChangedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeMovedSignature);

// UI 갱신용 델리게이트 (블루프린트 스킬 객체 안에서 호출할 수 있도록 유지)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillChargeChangedSignature, int32, CurrentCharge, int32, MaxCharge);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillReadySignature);

UCLASS()
class CARDADVENTURE_API UCardGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// ===================================================
	// 🗺️ 맵 시스템 (Map System)
	// ===================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	TArray<FMapNode> MapNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	int32 CurrentNodeID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	TArray<int32> VisitedNodes;

	UFUNCTION(BlueprintPure, Category = "Map System")
	EMapNodeState GetNodeState(int32 TargetNodeID) const;

	UPROPERTY(BlueprintAssignable, Category = "Map System|Events")
	FOnNodeMovedSignature OnNodeMoved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
	int32 TotalLayers = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
	float NodeSpacingX = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Generation")
	float NodeSpacingY = 200.0f;

	UFUNCTION(BlueprintCallable, Category = "Map System")
	void GenerateTestMap();

	UFUNCTION(BlueprintPure, Category = "Map System")
	bool IsNodeAccessible(int32 TargetNodeID) const;

	UFUNCTION(BlueprintCallable, Category = "Map System")
	void MoveToNode(int32 TargetNodeID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	bool IsNodeCleared = false;

	UFUNCTION(BlueprintCallable, Category = "Map System")
	void CompleteCurrentNode();


	// ===================================================
	// 👤 플레이어 데이터 (Player Data)
	// ===================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Data")
	FPlayerData CurrentPlayer;

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void InitializePlayer(EPlayerClass CharClass);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Player Data")
	void AddCardToDeck(FName NewCardID);

	UPROPERTY(BlueprintAssignable, Category = "Player Data|Events")
	FOnPlayerStatChangedSignature OnPlayerStatChanged;

	// ===================================================
	// ⚡ 스킬 시스템 (Skill System) - 개편됨
	// ===================================================

	// ★ 디자이너가 에디터에서 세팅할 스킬 클래스 (붕어빵 틀)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stat|Skill")
	TSubclassOf<USkillBase> ActiveSkillClass;

	// ★ 게임 중 실제로 생성되어 돌아가는 스킬 객체 (구워진 붕어빵)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stat|Skill")
	USkillBase *ActiveSkillInstance;

	// ★ 스킬 인스턴스를 생성하는 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void InitializeSkill();

	// 스킬 UI 델리게이트 (유지 - 블루프린트 스킬 로직에서 UI를 갱신할 때 사용)
	UPROPERTY(BlueprintAssignable, Category = "Skill|Events")
	FOnSkillChargeChangedSignature OnSkillChargeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Events")
	FOnSkillReadySignature OnSkillReady;

	// 블루프린트 스킬 객체에서 UI를 갱신하고 싶을 때 이 함수를 호출합니다.
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void UpdateSkillUI(int32 CurrentCharge, int32 MaxCharge, bool bIsReady);

	// ===================================================
	// ❓ 이벤트 시스템 (Event System)
	// ===================================================
	UFUNCTION(BlueprintCallable, Category = "Event System")
	void ApplyEventReward(EEventRewardType RewardType, int32 RewardValue);

	UFUNCTION(BlueprintCallable, Category = "Event System")
	void RestAtCampfire();
};