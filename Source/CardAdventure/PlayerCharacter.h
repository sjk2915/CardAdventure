#pragma once

#include "CoreMinimal.h"
#include "CardCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class CARDADVENTURE_API APlayerCharacter : public ACardCharacter
{
	GENERATED_BODY()

public:
	// 플레이어 전용 데미지 처리 (GameInstance 연동용)
	virtual void TakeDamageCustom(int32 DamageAmount) override;
};