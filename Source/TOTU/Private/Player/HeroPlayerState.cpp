// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Player/HeroPlayerState.h"

#include "AbilitySystem/TOTUAttributeSetAccessor.h"
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
	DOREPLIFETIME(AHeroPlayerState, PlayerAttributes);
	DOREPLIFETIME(AHeroPlayerState, PlayerAttributeID);
}

void AHeroPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	AttributeModificationHandler = NewObject<UInv_AttributeModificationHandler>(this);
	CreateNewPlayerAttributeID();

	if (AttributeModificationHandler)
	{
		UTOTUAttributeSetAccessor* Accessor = NewObject<UTOTUAttributeSetAccessor>(this);
		AttributeModificationHandler->SetAttributeSetAccessor(Accessor);
	}
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
	OnLevelChangedDelegate.Broadcast(Level);
}

void AHeroPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AHeroPlayerState::AddToAbilityPoints(int32 InPoints)
{
	AbilityPoints += InPoints;
	OnAbilityPointChangedDelegate.Broadcast(AbilityPoints);
}

void AHeroPlayerState::CreateNewPlayerAttributeID()
{
	PlayerAttributeID = FGuid::NewGuid();
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
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AHeroPlayerState::OnRep_AbilityPoints(int32 OldAbilityPoints)
{
	OnAbilityPointChangedDelegate.Broadcast(AbilityPoints);
}

void AHeroPlayerState::OnRep_PlayerAttributes(FPlayerAttributes OldPlayerAttributes)
{
	OnAttributesChangedDelegate.Broadcast(PlayerAttributes);
}

void AHeroPlayerState::OnRep_PlayerAttributeID(FGuid OldPlayerAttributeID)
{
	OnAttributeIDChangedDelegate.Broadcast(PlayerAttributeID);
}
#pragma endregion
/*-------------------------------------------------------------------------*/