// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Player/HeroPlayerState.h"
#include "Net/UnrealNetwork.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerState.cpp_Constructor
AHeroPlayerState::AHeroPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTOTUAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UTOTUAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerState.cpp_Functions
void AHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHeroPlayerState, Level);
	DOREPLIFETIME(AHeroPlayerState, XP);
	DOREPLIFETIME(AHeroPlayerState, AttributePoints);
	DOREPLIFETIME(AHeroPlayerState, AbilityPoints);
}

void AHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeModificationHandler = NewObject<UInv_AttributeModificationHandler>(this);
}


UAbilitySystemComponent* AHeroPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHeroPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AHeroPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChangedDelegate.Broadcast(NewLevel);
}

void AHeroPlayerState::AddToXP(int32 InXp)
{
	XP += InXp;
	OnXPChangedDelegate.Broadcast(XP);
}

void AHeroPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(InLevel);
}

void AHeroPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointChangedDelegate.Broadcast(InPoints);
}

void AHeroPlayerState::AddToAbilityPoints(int32 InPoints)
{
	AbilityPoints += InPoints;
	OnAbilityPointChangedDelegate.Broadcast(InPoints);
}

void AHeroPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AHeroPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AHeroPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointChangedDelegate.Broadcast(AbilityPoints);
}

void AHeroPlayerState::OnRep_AbilityPoints(int32 OldAbilityPoints)
{
	OnAbilityPointChangedDelegate.Broadcast(AbilityPoints);
}
#pragma endregion
/*-------------------------------------------------------------------------*/