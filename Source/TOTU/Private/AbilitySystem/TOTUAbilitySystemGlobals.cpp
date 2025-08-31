// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/TOTUAbilitySystemGlobals.h"
#include "TOTUGameplayAbilityTypes.h"
/*-------------------------------------------------------------------------*/

FGameplayEffectContext* UTOTUAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FTOTUGameplayEffectContext();
}
