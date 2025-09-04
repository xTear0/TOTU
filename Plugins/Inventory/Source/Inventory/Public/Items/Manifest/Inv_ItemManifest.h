// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Types/Inv_EnumTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "Inv_ItemManifest.generated.h"
/*-------------------------------------------------------------------------*/



// The Item Manifest contains all of the necessary data for creating
// a new Inventory Item.



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class UInv_InventoryItem;
class UInv_ItemDataAsset;
class UInv_CompositeBase;
class UInv_ItemComponent;
struct FInv_ItemFragment;

USTRUCT(BlueprintType)
struct FInv_ItemAttributePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories= "Attributes" ))
	FGameplayTag AttributeTag{FGameplayTag::EmptyTag};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value{0};
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories= "FragmentTags.StatMod" ))
	FGameplayTag FragmentTag{FGameplayTag::EmptyTag};
};

USTRUCT(BlueprintType)
struct FInv_ItemAbilityPair
{
	GENERATED_BODY()

	// TODO: Make this do something and mean something.
};

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()

	TArray<TInstancedStruct<FInv_ItemFragment>>& GetFragmentsMutable() { return DisplayFragments; }
	
	// Getters for Equipped Item Payload
	FText GetItemName() const { return ItemName; }
	FText GetItemDescription() const { return ItemDescription; }\
	EInv_ItemStar GetItemStars() const { return ItemStars; }
	TArray<FInv_ItemAttributePair> GetItemAttributes() const { return ItemAttributesContainer; }
	TArray<FInv_ItemAbilityPair> GetItemAbilities() const { return ItemAbilitiesContainer; }

	UInv_InventoryItem* Manifest(UObject* NewOuter, UInv_ItemComponent* OwningComponent);
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	EInv_ItemRarity GetItemRarity() const { return ItemRarity; }
	int32 GetItemSellValue() const { return SellValue; }
	FGameplayTag GetItemType() const { return ItemType; }
	int32 GetItemAttributeValue(FGameplayTag AttributeTag) const;
	
	// Create a reference back to the Owning Manifest for the Fragments.
	void ForEachFragmentSetOwningManifest(FInv_ItemManifest* InManifest); // Sets all Fragment Owning Manifest
	
	void AssimilateInventoryFragments(UInv_CompositeBase* Composite) const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

	template<typename T> requires std::derived_from<T, FInv_ItemFragment>
	TArray<const T*> GetAllFragmentsOfType() const;
	
	// Item Manifest can Spawn PickUp Actor:
	void SpawnPickupActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UInv_ItemDataAsset> SourceItemDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 EntryIndex = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText ItemName;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemStar ItemStars{EInv_ItemStar::OneStar};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FInv_ItemAttributePair> ItemAttributesContainer;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FInv_ItemAbilityPair> ItemAbilitiesContainer;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemRarity ItemRarity{EInv_ItemRarity::Common};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="TOTUItems"))
	FGameplayTag ItemType;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SellValue = 0;

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> DisplayFragments;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<AActor> PickupActorClass;

	void InitializeFromDataAsset();
	void ClearFragments();
};
/*-------------------------------------------------------------------------*/

template<typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : DisplayFragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : DisplayFragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
T* FInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : DisplayFragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	return nullptr;
}

template <typename T> requires std::derived_from<T, FInv_ItemFragment>
TArray<const T*> FInv_ItemManifest::GetAllFragmentsOfType() const
{
	TArray<const T*> Result;
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : DisplayFragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			Result.Add(FragmentPtr);
		}
	}
	return Result;
}
