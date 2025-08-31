// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "TOTU/Public/AbilitySystem/TOTUAttributeSet.h"
#include "Inv_EquippedItemPayload.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Inv_AttributeModificationHandler.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemPayloadDataEntry
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FName ItemName = "null";
    
    UPROPERTY(BlueprintReadOnly)
    FLinearColor ItemColor =  FLinearColor::White;

    UPROPERTY(BlueprintReadOnly)
    int32 AttributeValue{0};
};

USTRUCT(BlueprintType)
struct INVENTORY_API FHoverInspectResult
{
    GENERATED_BODY()

    // This array is filled with valid entries of items modifying an attribute.
    UPROPERTY(BlueprintReadOnly) 
    TArray<FInv_ItemPayloadDataEntry> PayloadEntries; 

    // This int32 is the final value of all the payloads added up.
    UPROPERTY(BlueprintReadOnly)
    int32 TotalAttributeValue{0};
};
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Class Functionality                                                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.h_Class
UCLASS(BlueprintType, Blueprintable)
class INVENTORY_API UInv_AttributeModificationHandler : public UObject
{
    GENERATED_BODY()

public:
    
/*-------------------------------------------------------------------------*/
/*   Payload Management Functions                                          */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.h_AttributeExporting
    
    UFUNCTION() // TODO: Not sure if this needs the macro tbh, tbd.
    void AddPayload(const FGuid& PayloadID, const FInv_EquippedItemPayload& Payload);
    
    UFUNCTION() // TODO: Not sure if this needs the macro tbh, tbd.
    void RemovePayload(const FGuid& PayloadID);
    
#pragma endregion
/*-------------------------------------------------------------------------*/

    
    
/*-------------------------------------------------------------------------*/
/*   Payload Data Querying Functions                                       */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.h_PayloadQueryingFunctions
    TArray<FInv_ItemPayloadDataEntry> QueryPayloadAttributeData(const FGameplayTag& Attribute);
        // Helper function. Returns an array of FInv_ItemAttributeDataEntries. These contain info about the payloads
        // which modify a specific attribute.

    int32 QueryPayloadAttributeValue(const FGameplayTag& Attribute);
        // Helper function. Returns an int32 total value of an attribute from all payloads.

    UFUNCTION(BlueprintCallable, Category = "Inv_AttributeModificationHandler")
    FHoverInspectResult HoverInspectAttribute(const FGameplayTag& Attribute);
        // This function returns an FHoverInspectResult which pulls information from the active payloads to give
        // information about the payloads modifying a specific attribute.
#pragma endregion
/*-------------------------------------------------------------------------*/


    
/*-------------------------------------------------------------------------*/
/*   Attribute Exporting Functions                                         */
/*-------------------------------------------------------------------------*/
#pragma region Inv_AttributeModificationHandler.h_AttributeExporting
    
    void CycleGameplayEffectFromPayloads();
        void RemoveGameplayEffect();    // Called first.
        void ApplyGameplayEffect();     // Called second.

#pragma endregion
/*-------------------------------------------------------------------------*/

    
private:

    FGameplayAttribute GetAttributeFromTag(const FGameplayTag& AttributeTag) const;
    
    // Payload storage
    UPROPERTY()
    TMap<FString, FInv_EquippedItemPayload> ActivePayloads;

    UPROPERTY()
    TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
    
    FActiveGameplayEffectHandle ActiveEffectHandle;
    
};
#pragma endregion
/*-------------------------------------------------------------------------*/