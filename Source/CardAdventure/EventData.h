#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EventData.generated.h"

// 선택지의 결과 타입
UENUM(BlueprintType)
enum class EEventRewardType : uint8
{
	None		UMETA(DisplayName = "아무 일도 없음"),
	TakeDamage	UMETA(DisplayName = "체력 감소 (피해)"),
	HealHP		UMETA(DisplayName = "체력 회복"),
	GainGold	UMETA(DisplayName = "골드 획득"),
	LoseGold	UMETA(DisplayName = "골드 감소")
	// 나중에 GainCard(카드 획득), GainRelic(유물 획득) 등을 추가하시면 됩니다.
};

// 개별 선택지 데이터
USTRUCT(BlueprintType)
struct FEventChoice
{
	GENERATED_BODY()

	// 1. 선택지 제목 (예: "산으로 간다", "강으로 간다")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FText ChoiceTitle;

	// 2. 선택 효과 설명 (예: "체력을 9 잃습니다.", "체력을 9 얻습니다.")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (MultiLine = "true"))
	FText ChoiceDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	EEventRewardType RewardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	int32 RewardValue;
};

// 데이터 테이블에 들어갈 이벤트 본문
USTRUCT(BlueprintType)
struct FEventData : public FTableRowBase
{
	GENERATED_BODY()

	// 1. 이벤트 제목 (예: "산과 강", "음흉한 상인")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FText EventTitle;

	// 2. 이벤트 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (MultiLine = "true"))
	FText EventDescription;

	// 이벤트 배경 스프라이트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TSoftObjectPtr<class UPaperSprite> EventSprite;

	// 하나의 이벤트가 가질 여러 개의 선택지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TArray<FEventChoice> Choices;
};