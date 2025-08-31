// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "TOTUAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "TOTUGameplayTags.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAssetManager.cpp_Functions
UTOTUAssetManager& UTOTUAssetManager::Get()
{
	check(GEngine);
	UTOTUAssetManager* TOTUAssetManager = Cast<UTOTUAssetManager>(GEngine->AssetManager);
	return *TOTUAssetManager;
}
void UTOTUAssetManager::StartInitialLoading()
{FTOTUGameplayTags::Get();
	Super::StartInitialLoading();
	FTOTUGameplayTags::Get();

	//Required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
#pragma endregion
/*-------------------------------------------------------------------------*/