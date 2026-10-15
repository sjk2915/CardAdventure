#include "PlayerCharacter.h"
#include "CardGameInstance.h"
#include "Kismet/GameplayStatics.h"

void APlayerCharacter::TakeDamageCustom(int32 DamageAmount)
{
	// 1. 방어도 차감 및 로컬 체력 감소는 부모(CardCharacter)의 기존 로직을 그대로 사용합니다.
	Super::TakeDamageCustom(DamageAmount);

	// 2. 부모 로직 실행 후 변경된 최종 체력을 GameInstance에 동기화합니다.
	if (UCardGameInstance *GameInstance = Cast<UCardGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		// 로컬 체력을 글로벌(GameInstance) 체력에 덮어쓰기
		GameInstance->CurrentPlayer.CurrentHP = this->CurrentHealth;

		// 글로벌 HUD 갱신 신호 발송!
		GameInstance->OnPlayerStatChanged.Broadcast();
	}
}