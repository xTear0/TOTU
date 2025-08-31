// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Character/TOTUCharacterBase.h"
#include "TOTU/TOTU.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "TOTU/Public/TOTUGameplayTags.h"
#include "Components/CapsuleComponent.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region TOTUCharacterBase.cpp_Constructor
ATOTUCharacterBase::ATOTUCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	InitialLifeSpan = 0.f;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region TOTUCharacterBase.cpp_Functions
UAbilitySystemComponent* ATOTUCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ATOTUCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ATOTUCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void ATOTUCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void ATOTUCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector ATOTUCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FTOTUGameplayTags& GameplayTags = FTOTUGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		if (TargetMeshForAttachment) return TargetMeshForAttachment->GetSocketLocation(RightHandSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		if (TargetMeshForAttachment) return TargetMeshForAttachment->GetSocketLocation(LeftHandSocketName);
	}
	return FVector();
}

bool ATOTUCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ATOTUCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ATOTUCharacterBase::GetAttackMontages_Implementation()
{	
	return AttackMontages;
}

UNiagaraSystem* ATOTUCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

ECharacterClass ATOTUCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

void ATOTUCharacterBase::AttachWeaponToSelectedMesh(USkeletalMeshComponent* WeaponMesh, FName SocketName)
{
	if (Weapon && TargetMeshForAttachment)
	{
		Weapon->AttachToComponent(TargetMeshForAttachment, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		Weapon->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	}
}

void ATOTUCharacterBase::InitAbilityActorInfo()
{
}

void ATOTUCharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATOTUCharacterBase::InitializeDefaultAttributes() const {}

void ATOTUCharacterBase::AddCharacterAbilities()
{
	UTOTUAbilitySystemComponent* TOTUASC = CastChecked<UTOTUAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	TOTUASC->AddCharacterAbilities(StartupAbilities);
	TOTUASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}
void ATOTUCharacterBase::Dissolve()
{
	if (IsValid(CharacterDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			CharacterDissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartCharacterDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/