// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/Data/CharacterClassInfo.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region CharacterClassInfo.cpp_Functions
FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
#pragma endregion
/*-------------------------------------------------------------------------*/