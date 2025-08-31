// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TOTUGameplayAbility.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality                                                    */
/*-------------------------------------------------------------------------*/
#pragma region TOTUGameplayAbility.h_Class
UCLASS()
class TOTU_API UTOTUGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public: 

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

};
#pragma endregion
/*-------------------------------------------------------------------------*/