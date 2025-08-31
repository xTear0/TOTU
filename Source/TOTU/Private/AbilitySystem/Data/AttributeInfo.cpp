// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/Data/AttributeInfo.h"
#include "TOTU/TOTULogChannels.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region AttributeInfo.cpp_Class
FTOTUAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FTOTUAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTOTU, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"),
			*AttributeTag.ToString(),
			*GetNameSafe(this));
	}
	return FTOTUAttributeInfo();
}
#pragma endregion
/*-------------------------------------------------------------------------*/
