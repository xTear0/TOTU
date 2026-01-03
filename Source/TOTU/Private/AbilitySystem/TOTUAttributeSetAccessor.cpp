// Copyright xTear Studios
#include "AbilitySystem/TOTUAttributeSetAccessor.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "TOTUGameplayTags.h"
#include "GameplayTagContainer.h"

FGameplayAttribute UTOTUAttributeSetAccessor::GetAttributeFromTag_Implementation(const FGameplayTag& AttributeTag) const
{
    // Primary Attributes
    if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Strength"), false)))
        return UTOTUAttributeSet::GetStrengthAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Intelligence"), false)))
        return UTOTUAttributeSet::GetIntelligenceAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Fortitude"), false)))
        return UTOTUAttributeSet::GetFortitudeAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Vitality"), false)))
        return UTOTUAttributeSet::GetVitalityAttribute();

    // Secondary Attributes
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Primary.Dexterity"), false)))
        return UTOTUAttributeSet::GetDexterityAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.MaxMana"), false)))
        return UTOTUAttributeSet::GetMaxManaAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.ManaRegen"), false)))
        return UTOTUAttributeSet::GetManaRegenAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.MaxHealth"), false)))
        return UTOTUAttributeSet::GetMaxHealthAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.HealthRegen"), false)))
        return UTOTUAttributeSet::GetHealthRegenAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.CritChance"), false)))
        return UTOTUAttributeSet::GetCritChanceAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.CritDamage"), false)))
        return UTOTUAttributeSet::GetCritDamageAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Secondary.IgnoreDEF"), false)))
        return UTOTUAttributeSet::GetIgnoreDEFAttribute();

    // Resistances
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance"), false)))
        return UTOTUAttributeSet::GetResistanceAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Physical"), false)))
        return UTOTUAttributeSet::GetPhysicalResAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Poison"), false)))
        return UTOTUAttributeSet::GetPoisonResAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Ice"), false)))
        return UTOTUAttributeSet::GetIceResAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Fire"), false)))
        return UTOTUAttributeSet::GetFireResAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Light"), false)))
        return UTOTUAttributeSet::GetLightResAttribute();
    else if (AttributeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Shadow"), false)))
        return UTOTUAttributeSet::GetShadowResAttribute();

    return FGameplayAttribute();
}