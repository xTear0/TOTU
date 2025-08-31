// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TOTUAssetManager.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality                                                    */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAssetManager.h_Class
UCLASS()
class TOTU_API UTOTUAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UTOTUAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;


};
#pragma endregion
/*-------------------------------------------------------------------------*/