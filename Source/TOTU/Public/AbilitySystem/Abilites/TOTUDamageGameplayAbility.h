// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/TOTUGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "TOTUDamageGameplayAbility.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality                                                    */
/*-------------------------------------------------------------------------*/
#pragma region TOTUDamageGameplayAbility.h_Class
UCLASS()
class TOTU_API UTOTUDamageGameplayAbility : public UTOTUGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)	
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
#pragma endregion
/*-------------------------------------------------------------------------*/