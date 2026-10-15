#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardGameInstance.h"
#include "CardBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CARDADVENTURE_API UCardBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// ★ 핵심: BlueprintPure로 선언하여 실행 핀(하얀색 선)이 없는 초록색 노드로 만듭니다!
	// meta = (WorldContext = "WorldContextObject")를 넣어야 블루프린트에서 WorldContext 핀이 숨겨집니다.
	UFUNCTION(BlueprintPure, Category = "Card System", meta = (WorldContext = "WorldContextObject"))
	static UCardGameInstance *GetCardGameInstance(const UObject *WorldContextObject);
};
