// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Character/HeroCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/HeroPlayerController.h"
#include "Player/HeroPlayerState.h"
#include "UI/HUD/TOTUHUD.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region HeroCharacter.cpp_Constructor
AHeroCharacter::AHeroCharacter()
{
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	Weapon->SetVisibility(false);
	CharacterClass = ECharacterClass::Arcanist;
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region HeroCharacter.cpp_Functions
void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init AbilityActor for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();

}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void AHeroCharacter::AddToXP_Implementation(int32 InXP)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToXP(InXP);
}

void AHeroCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AHeroCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AHeroCharacter::GetXP_Implementation() const
{
	const AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetXP();
}

int32 AHeroCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AHeroCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->LevelUpInfo->LevelUpInformation[Level].AbilityPointAward;
}

int32 AHeroCharacter::GetAbilityPointsReward_Implementation(int32 Level) const
{
	const AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->LevelUpInfo->LevelUpInformation[Level].AbilityPointAward;
}

void AHeroCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToLevel(InPlayerLevel);
}

void AHeroCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToAttributePoints(InAttributePoints);
}

void AHeroCharacter::AddToAbilityPoints_Implementation(int32 InAbilityPoints)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToAbilityPoints(InAbilityPoints);
}

int32 AHeroCharacter::GetAttributePoints_Implementation() const
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetAttributePoints();
}

int32 AHeroCharacter::GetAbilityPoints_Implementation() const
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetAbilityPoints();
}

FPlayerAttributes AHeroCharacter::GetPlayerAttributes_Implementation() const
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetPlayerAttributes();
}

void AHeroCharacter::CreateNewPlayerAttributeID_Implementation()
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->CreateNewPlayerAttributeID();
}

FGuid AHeroCharacter::GetPlayerAttributeID_Implementation() const
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetPlayerAttributeID();
}

void AHeroCharacter::SetStrength_Implementation(int32 NewStrength)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->SetStrength(NewStrength);
}

void AHeroCharacter::SetIntelligence_Implementation(int32 NewIntelligence)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->SetIntelligence(NewIntelligence);
}

void AHeroCharacter::SetFortitude_Implementation(int32 NewFortitude)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->SetFortitude(NewFortitude);
}

void AHeroCharacter::SetVitality_Implementation(int32 NewVitality)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->SetVitality(NewVitality);
}

void AHeroCharacter::AddToStrength_Implementation(int32 InStrength)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToStrength(InStrength);
}

void AHeroCharacter::AddToIntelligence_Implementation(int32 InIntelligence)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToIntelligence(InIntelligence);
}

void AHeroCharacter::AddToFortitude_Implementation(int32 InFortitude)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToFortitude(InFortitude);
}

void AHeroCharacter::AddToVitality_Implementation(int32 InVitality)
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->AddToVitality(InVitality);
}

int32 AHeroCharacter::GetPlayerLevel_Implementation()
{
	const AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	return HeroPlayerState->GetPlayerLevel();
}

void AHeroCharacter::InitializeDefaultAttributes() const
{
	ApplyGameplayEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyGameplayEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyGameplayEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AHeroCharacter::InitAbilityActorInfo()
{
	AHeroPlayerState* HeroPlayerState = GetPlayerState<AHeroPlayerState>();
	check(HeroPlayerState);
	HeroPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(HeroPlayerState, this);

	Cast<UTOTUAbilitySystemComponent>(HeroPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = HeroPlayerState->GetAbilitySystemComponent();
	AttributeSet = HeroPlayerState->GetAttributeSet();

	auto foo = GetController();

	if (AHeroPlayerController* HeroPlayerController = Cast<AHeroPlayerController>(GetController()))
	{
		if (ATOTUHUD* TOTUHUD = Cast<ATOTUHUD>(HeroPlayerController->GetHUD()))
		{
			TOTUHUD->InitOverlay(HeroPlayerController, HeroPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
#pragma endregion
/*-------------------------------------------------------------------------*/