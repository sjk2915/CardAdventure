#include "SkillBase.h"

UWorld *USkillBase::GetWorld() const
{
	// CDO(기본 객체)이거나 외부(Outer)가 없으면 nullptr, 있으면 Outer의 월드를 반환
	if (HasAnyFlags(RF_ClassDefaultObject)) return nullptr;
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}