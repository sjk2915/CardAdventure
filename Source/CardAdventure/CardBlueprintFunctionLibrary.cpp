#include "CardBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UCardGameInstance *UCardBlueprintFunctionLibrary::GetCardGameInstance(const UObject *WorldContextObject)
{
	// 백엔드(C++)에서 대신 캐스팅을 한 번 해주고 결과를 던져줍니다.
	if (UGameInstance *GI = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return Cast<UCardGameInstance>(GI);
	}
	return nullptr;
}