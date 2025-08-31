// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Actor/TOTUProjectile.h"
#include "TOTU/TOTU.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TOTUAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region TOTUProjectile.cpp_Constructor
ATOTUProjectile::ATOTUProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region TOTUProjectile.cpp_Functions
void ATOTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATOTUProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

}

void ATOTUProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent)	LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void ATOTUProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return; // Either the data is not valid or the actor that cast the projectile and the actor the projectile hit are the same.
	}
	if (!UTOTUAbilitySystemLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return; // The actor that cast the projectile and the actor the projectile hit are on the same team.
	}

	if (!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (LoopingSoundComponent)	LoopingSoundComponent->Stop();
	}

	if (HasAuthority())
	{
		// Impact radius will ignore all actors with the same tag.
		if (DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser()->ActorHasTag(FName("Player"))) TagToIgnore = FName("Player");
		if (DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser()->ActorHasTag(FName("Enemy"))) TagToIgnore = FName("Enemy");

		UTOTUAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OutOverlappingActors, IgnoredActors, OverlapHitRadius, GetActorLocation());
		
		for (AActor* ActorToHit : OutOverlappingActors)
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorToHit))
			{
				if (!ActorToHit->ActorHasTag(TagToIgnore)) TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			}
		}
		Destroy();
	}
	else {
		bHit = true;
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/