// Copyright xTear 
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/Data/AbilityInfo.h"

#include "TOTU/TOTULogChannels.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region AbilityInfo.cpp_Class
FTOTUAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for (const FTOTUAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTOTU, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"),
			*AbilityTag.ToString(),
			*GetNameSafe(this));
	}
	return FTOTUAbilityInfo();
}
#pragma endregion
/*-------------------------------------------------------------------------*/
