// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/TOTUDamageGameplayAbility.h"
#include "TOTUProjectileSpell.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class ATOTUProjectile;
class UGameplayEffect;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Spell Class                                                           */
/*-------------------------------------------------------------------------*/
#pragma region TOTUProjectileSpell.h_Class
UCLASS()
class TOTU_API UTOTUProjectileSpell : public UTOTUDamageGameplayAbility	
{
	GENERATED_BODY()	
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, bool bCastRightHandSlot, bool bCastLeftHandSlot, bool bCastWeaponSlot);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATOTUProjectile> ProjectileClass;
};
#pragma endregion
/*-------------------------------------------------------------------------*/