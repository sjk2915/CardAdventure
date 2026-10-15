#pragma once

#include "CoreMinimal.h"
#include "MapData.generated.h"

// 방(노드)의 상태를 정의
UENUM(BlueprintType)
enum class EMapNodeState : uint8
{
	Locked      UMETA(DisplayName = "비활성 (Locked)"),
	Accessible  UMETA(DisplayName = "이동 가능 (Accessible)"),
	Current     UMETA(DisplayName = "현재 노드 (Current)"),
	Cleared     UMETA(DisplayName = "클리어됨 (Cleared)")
};

// 방(노드)의 종류를 정의
UENUM(BlueprintType)
enum class ENodeType : uint8
{
	Combat      UMETA(DisplayName = "일반 전투 (Combat)"),
	Elite       UMETA(DisplayName = "엘리트 전투 (Elite)"),
	Rest		UMETA(DisplayName = "휴식처 (Rest)"),
	Shop        UMETA(DisplayName = "상점 (Shop)"),
	Unknown     UMETA(DisplayName = "물음표 (Unknown)"),
	Boss        UMETA(DisplayName = "보스 (Boss)")
};

// 맵 노드 하나의 데이터를 담는 구조체
USTRUCT(BlueprintType)
struct FMapNode
{
	GENERATED_BODY()

	// 현재 노드의 고유 ID (예: 0, 1, 2...)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	int32 NodeID = -1;

	// 이 노드의 방 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	ENodeType NodeType = ENodeType::Combat;

	// 이 노드에서 다음으로 이동할 수 있는 노드들의 ID 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	TArray<int32> ConnectedNodes;

	// 현재 맵 화면에서 노드의 시각적 위치 (X, Y 좌표)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapData")
	FVector2D Position = FVector2D::ZeroVector;
};