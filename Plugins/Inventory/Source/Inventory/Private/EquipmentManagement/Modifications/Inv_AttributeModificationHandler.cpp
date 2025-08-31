// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "EquipmentManagement/Modifications/Inv_AttributeModificationHandler.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "TOTU/Public/Player/HeroPlayerState.h"
#include "TOTU/Public/TOTUGameplayTags.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Payload Management                                                    */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.cpp_PayloadManagement
void UInv_AttributeModificationHandler::AddPayload(const FGuid& PayloadID, const FInv_EquippedItemPayload& Payload)
{
    ActivePayloads.Add(PayloadID.ToString(), Payload);
    CycleGameplayEffectFromPayloads();
}

void UInv_AttributeModificationHandler::RemovePayload(const FGuid& PayloadID)
{
    ActivePayloads.Remove(PayloadID.ToString());
    CycleGameplayEffectFromPayloads();
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Payload Data Querying Functions                                       */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.cpp_PayloadQuerying
TArray<FInv_ItemPayloadDataEntry> UInv_AttributeModificationHandler::QueryPayloadAttributeData(
    const FGameplayTag& Attribute)
{
    TArray<FInv_ItemPayloadDataEntry> Result;
    
    // Reserve space to minimize reallocations.
    Result.Reserve(ActivePayloads.Num());
    
    // Iterate over all active payloads in the TMap.
    for (const auto& PayloadPair : ActivePayloads)
    {
        const FInv_EquippedItemPayload& Payload = PayloadPair.Value;
        
        for (const FInv_ItemAttributePair& AttributePair : Payload.ItemAttributesContainer)
        {
            if (AttributePair.AttributeTag.MatchesTagExact(Attribute))
            {
                FInv_ItemPayloadDataEntry& NewEntry = Result.AddDefaulted_GetRef();
                NewEntry.ItemName = Payload.ItemName;
                NewEntry.ItemColor = Payload.ItemColor;
                NewEntry.AttributeValue = AttributePair.Value;
            }
        }
    }
    
    // Shrink the array to fit only the actual number of results.
    Result.Shrink();
    
    return Result;
}

int32 UInv_AttributeModificationHandler::QueryPayloadAttributeValue(const FGameplayTag& Attribute)
{
    int32 Result = 0;
    // Iterate over all active payloads in the TMap.
    for (const auto& PayloadPair : ActivePayloads)
    {
        const FInv_EquippedItemPayload& Payload = PayloadPair.Value;
        for (const FInv_ItemAttributePair& AttributePair : Payload.ItemAttributesContainer)
        {
            if (AttributePair.AttributeTag.MatchesTagExact(Attribute))
            {
                Result += AttributePair.Value;
            }
        }
    }
    return Result;
}

FHoverInspectResult UInv_AttributeModificationHandler::HoverInspectAttribute(const FGameplayTag& Attribute)
{
    FHoverInspectResult Result;                                             // Create Result
    
    Result.PayloadEntries = QueryPayloadAttributeData(Attribute);           // Fill in Payload Entries
    Result.TotalAttributeValue = QueryPayloadAttributeValue(Attribute);     // Fill in total Attribute Value
    
    return Result;                                                          // Return Result.
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Attribute Exporting Functions                                         */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.cpp_AttributeExporting
void UInv_AttributeModificationHandler::CycleGameplayEffectFromPayloads()
{
    RemoveGameplayEffect();
    ApplyGameplayEffect();
}

void UInv_AttributeModificationHandler::RemoveGameplayEffect()
{
    // Check if we have a valid effect handle and ASC
    if (ActiveEffectHandle.IsValid() && CachedASC.IsValid())
    {
        CachedASC->RemoveActiveGameplayEffect(ActiveEffectHandle);
    }
    
    // Reset the handle regardless of whether removal was successful
    ActiveEffectHandle = FActiveGameplayEffectHandle();
}

void UInv_AttributeModificationHandler::ApplyGameplayEffect()
{
    // Get the Player State (assuming this object is owned by Player State)
    AHeroPlayerState* HeroPlayerState = GetTypedOuter<AHeroPlayerState>();
    if (!HeroPlayerState)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttributeModificationHandler: Could not find Player State"));
        return;
    }
    
    // Get and cache the Ability System Component
    UAbilitySystemComponent* ASC = HeroPlayerState->GetAbilitySystemComponent();
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("AttributeModificationHandler: Player State has no Ability System Component"));
        return;
    }
    CachedASC = ASC;
    
    // Create a dynamic gameplay effect
    UGameplayEffect* DynamicEffect = NewObject<UGameplayEffect>(GetTransientPackage());
    if (!DynamicEffect)
    {
        UE_LOG(LogTemp, Error, TEXT("AttributeModificationHandler: Failed to create dynamic gameplay effect"));
        return;
    }
    
    // Configure the effect for infinite duration
    DynamicEffect->DurationPolicy = EGameplayEffectDurationType::Infinite;
    DynamicEffect->Modifiers.Empty(); // Clear any existing modifiers
    
    // Collect all unique attributes and their total values from active payloads
    TMap<FGameplayTag, int32> AttributeTotals;
    
    for (const auto& PayloadPair : ActivePayloads)
    {
        const FInv_EquippedItemPayload& Payload = PayloadPair.Value;
        
        for (const FInv_ItemAttributePair& AttributePair : Payload.ItemAttributesContainer)
        {
            if (AttributeTotals.Contains(AttributePair.AttributeTag))
            {
                AttributeTotals[AttributePair.AttributeTag] += AttributePair.Value;
            }
            else
            {
                AttributeTotals.Add(AttributePair.AttributeTag, AttributePair.Value);
            }
        }
    }
    
    for (const auto& AttributeTotal : AttributeTotals)
    {
        // Skip attributes with zero modification
        if (AttributeTotal.Value == 0)
        {
            continue;
        }
        
        // Create a new modifier
        FGameplayModifierInfo NewModifier;
        
        // Set the attribute to modify - resolve tag to actual attribute
        FGameplayAttribute GameplayAttribute = GetAttributeFromTag(AttributeTotal.Key);
        if (!GameplayAttribute.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("AttributeModificationHandler: Invalid attribute for tag %s"), *AttributeTotal.Key.ToString());
            continue;
        }
        NewModifier.Attribute = GameplayAttribute;
        
        // Set the operation (additive modification)
        NewModifier.ModifierOp = EGameplayModOp::Additive;
        
        // Set the magnitude using a simple scalar float
        FScalableFloat Magnitude;
        Magnitude.Value = static_cast<float>(AttributeTotal.Value);
        
        NewModifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(Magnitude);
        
        // Add the modifier to the effect
        DynamicEffect->Modifiers.Add(NewModifier);
    }
    
    // Only apply the effect if we have modifiers
    if (DynamicEffect->Modifiers.Num() > 0)
    {
        // Apply the effect directly using the class
        FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
        ActiveEffectHandle = ASC->ApplyGameplayEffectToSelf(DynamicEffect, 1.0f, ContextHandle);
        
        if (!ActiveEffectHandle.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("AttributeModificationHandler: Failed to apply gameplay effect. Modifiers: %d"), DynamicEffect->Modifiers.Num());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("AttributeModificationHandler: Successfully applied gameplay effect with %d modifiers"), DynamicEffect->Modifiers.Num());
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("AttributeModificationHandler: No modifiers to apply"));
    }
}

FGameplayAttribute UInv_AttributeModificationHandler::GetAttributeFromTag(const FGameplayTag& AttributeTag) const
{
    {
        // Fetch the attribute set from the ASC.
        if (!CachedASC.IsValid())
        {
            return FGameplayAttribute();
        }
        
        const UTOTUAttributeSet* AttributeSet = CachedASC->GetSet<UTOTUAttributeSet>();
        if (!AttributeSet)
        {
            return FGameplayAttribute();
        }
        
        if (const auto* FunctionPtr = AttributeSet->TagsToAttributes.Find(AttributeTag))
        {
            if (*FunctionPtr)
            {
                return (*FunctionPtr)();
            }
        }
        return FGameplayAttribute(); // Invalid attribute if not found
    }
}
#pragma endregion
/*-------------------------------------------------------------------------*/