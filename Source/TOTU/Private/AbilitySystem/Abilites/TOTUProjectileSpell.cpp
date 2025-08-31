// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/Abilites/TOTUProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/TOTUProjectile.h"
#include "TOTU/Public/TOTUGameplayTags.h"
#include "Interaction/CombatInterface.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region TOTUProjectileSpell.cpp_Class
void UTOTUProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
									const FGameplayAbilityActorInfo* ActorInfo, 
									const FGameplayAbilityActivationInfo ActivationInfo, 
									const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}
void UTOTUProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, bool bCastRightHandSlot, bool bCastLeftHandSlot, bool bCastWeaponSlot)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;


	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector LeftHandCastSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		FTOTUGameplayTags::Get().CombatSocket_LeftHand);
	const FVector RightHandCastSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		FTOTUGameplayTags::Get().CombatSocket_RightHand);
	const FVector WeaponTipSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		FTOTUGameplayTags::Get().CombatSocket_Weapon);

	FVector SocketLocation;
	if (bCastRightHandSlot && !bCastLeftHandSlot && !bCastWeaponSlot) SocketLocation = RightHandCastSocketLocation;
	if (bCastLeftHandSlot && !bCastRightHandSlot && !bCastWeaponSlot) SocketLocation = LeftHandCastSocketLocation;
	if (bCastWeaponSlot && !bCastLeftHandSlot && !bCastRightHandSlot) SocketLocation = WeaponTipSocketLocation;
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ATOTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATOTUProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

	const FTOTUGameplayTags GameplayTags = FTOTUGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}
#pragma endregion
/*-------------------------------------------------------------------------*/