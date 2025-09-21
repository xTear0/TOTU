// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "AbilitySystem/Abilites/TOTUGameplayAbility.h"
#include "TOTUGameplayTags.h"
#include "GenericPlatform/GenericPlatformSymbolication.h"
#include "Interaction/PlayerInterface.h"
#include "Player/HeroPlayerState.h"
#include "TOTU/TOTULogChannels.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions										                         */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAbilitySystemComponent.cpp_Functions
void UTOTUAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTOTUAbilitySystemComponent::ClientEffectApplied);
}

void UTOTUAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UTOTUGameplayAbility* TOTUAbility = Cast<UTOTUGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(TOTUAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitieGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UTOTUAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UTOTUAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UTOTUAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UTOTUAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTOTU, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UTOTUAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UTOTUAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UTOTUAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UTOTUAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	// This is the special sauce function that sends a payload to the attribute modification handler
	AHeroPlayerState* HeroPlayerState = GetAvatarActor() ? 
	GetAvatarActor()->GetOwner<APlayerController>()->GetPlayerState<AHeroPlayerState>() : 
	nullptr;
	UInv_AttributeModificationHandler* AttributeHandler = HeroPlayerState->GetAttributeModificationHandler();
	check (AttributeHandler);
	
	// Step 1: The Player State has a struct that tracks what attribute points the player has spent. Add new point.
	
	if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Strength"), false)))
	{
		IPlayerInterface::Execute_AddToStrength(GetAvatarActor(), 1);	
	}
	if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Intelligence"), false)))
	{
		IPlayerInterface::Execute_AddToIntelligence(GetAvatarActor(), 1);	
	}
	if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Fortitude"), false)))
	{
		IPlayerInterface::Execute_AddToFortitude(GetAvatarActor(), 1);	
	}
	if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Vitality"), false)))
	{
		IPlayerInterface::Execute_AddToVitality(GetAvatarActor(), 1);	
	}

	// Step 2: Get the struct that contains int values for every spent attribute point.
	FPlayerAttributes Attributes = IPlayerInterface::Execute_GetPlayerAttributes(GetAvatarActor());

	// Step 3: Create and export a payload to the PlayerState Attribute Modification Handler with the Player's Attribute Info.
	// A. Get Player State reference to the stored FGuid.
	FGuid PlayerAttributeID = IPlayerInterface::Execute_GetPlayerAttributeID(GetAvatarActor());
	// A.1 With a valid Attribute Handler, make sure we clean off the original payload.
	AttributeHandler->RemovePayload(PlayerAttributeID);
	
	// B. Create a payload.
	FInv_EquippedItemPayload Payload;
	TArray<FInv_ItemAttributePair> AttributePairs;

	Payload.ItemName = FName("Base Attributes");
	Payload.ItemColor = FLinearColor::FromSRGBColor(FColor::FromHex("FDD697FF"));
	Payload.PayloadID = PlayerAttributeID;

	// C. Fetch Attributes:
	FInv_ItemAttributePair BaseStrengthAttribute;
	BaseStrengthAttribute.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Strength"));
	BaseStrengthAttribute.Value = Attributes.PlayerStrength;

	FInv_ItemAttributePair BaseIntelligenceAttribute;
	BaseIntelligenceAttribute.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Intelligence"));
	BaseIntelligenceAttribute.Value = Attributes.PlayerIntelligence;

	FInv_ItemAttributePair BaseFortitudeAttribute;
	BaseFortitudeAttribute.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Fortitude"));
	BaseFortitudeAttribute.Value = Attributes.PlayerFortitude;

	FInv_ItemAttributePair BaseVitalityAttribute;
	BaseVitalityAttribute.AttributeTag = FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Vitality"));
	BaseVitalityAttribute.Value = Attributes.PlayerVitality;

	// D. Add Attributes:
	AttributePairs.Add(BaseStrengthAttribute);
	AttributePairs.Add(BaseIntelligenceAttribute);
	AttributePairs.Add(BaseFortitudeAttribute);
	AttributePairs.Add(BaseVitalityAttribute);
	Payload.ItemAttributesContainer = AttributePairs;

	// C. Add Payload
	if (IsValid(AttributeHandler)) AttributeHandler->AddPayload(Payload.PayloadID, Payload);


	// Step 4: Remove 1 Attribute Point from the Player State.
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UTOTUAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitieGiven)
	{
		bStartupAbilitieGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}

void UTOTUAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
#pragma endregion
/*-------------------------------------------------------------------------*/