// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Inv_ItemDataAsset.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region Inv_ItemManifest.cpp_Functions
UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter, UInv_ItemComponent* OwningComponent)
{
	// Load values from the data asset first
	if (EntryIndex != INDEX_NONE) InitializeFromDataAsset();
	ConstructManifestDisplayFragments();
	
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
	
	// Step 1: Copy the Manifest into the item.
	Item->SetItemManifest(*this);
	
	// Step 2: Set the Owning Component for all fragments on the copy.
	FInv_ItemManifest* ItemManifestMutableCopy = &Item->GetItemManifestMutable();
	ItemManifestMutableCopy->ForEachFragmentSetOwningManifest(ItemManifestMutableCopy);

	// Step 3: Call Manifest on each fragment on the copy.
	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();
	}

	// Step 4: Clear the original fragments.
	EmptyManifestDisplayFragments();
	
	return Item;
}

int32 FInv_ItemManifest::GetItemAttributeValue(FGameplayTag AttributeTag) const
{
	for (const FInv_ItemAttributePair& Pair : ItemAttributesContainer)
	{
		if (Pair.AttributeTag == AttributeTag)
		{
			return Pair.Value;
		}
	}
	return 0;
}

void FInv_ItemManifest::EmptyManifestDisplayFragments()
{
	for (auto& Fragment : DisplayFragments)
	{
		Fragment.Reset();
	}
	DisplayFragments.Empty();
}

void FInv_ItemManifest::AddExtraManifestDisplayFragments()
{
	if (ExtraFragments.IsEmpty()) return;
	for (auto& Fragment : ExtraFragments)
	{
		DisplayFragments.Add(Fragment);
	}
}

void FInv_ItemManifest::RefreshManifestDisplayFragments()
{
	EmptyManifestDisplayFragments();
	ConstructManifestDisplayFragments();
}

void FInv_ItemManifest::ConstructManifestDisplayFragments()
{
	EmptyManifestDisplayFragments();
	TryMakeItemNameDisplayFragment();
	TryMakeItemDescriptionDisplayFragment();
	TryMakeItemSellValueDisplayFragment();
	TryMakeItemStarsDisplayFragment();
	TryMakeItemTypeRarityDisplayFragment();
	TryMakeItemIconDisplayFragment();
	TryMakeItemGridDisplayFragment();
	AddExtraManifestDisplayFragments();
}

void FInv_ItemManifest::TryMakeItemNameDisplayFragment()
{
	if (ItemName.IsEmpty()) return;							// Empty, continue.
	
	FInv_TextFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.ItemNameFragment")));
	Fragment.SetText(ItemName);
	
	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_TextFragment>(Fragment));
}

void FInv_ItemManifest::TryMakeItemDescriptionDisplayFragment()
{
	if (ItemDescription.IsEmpty()) return;					// Empty, continue.
	FInv_TextFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.FlavorTextFragment")));
	Fragment.SetText(ItemDescription);
	
	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_TextFragment>(Fragment));
}

void FInv_ItemManifest::TryMakeItemTypeRarityDisplayFragment()
{
	// TODO: Optimize
	
	FInv_TextFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.ItemTypeFragment")));
	Fragment.bPrefixWithRarity = true;
	Fragment.SetText(UInv_WidgetUtils::GetItemTypeNameFromTag(ItemType));
	
	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_TextFragment>(Fragment));
}

void FInv_ItemManifest::TryMakeItemStarsDisplayFragment()
{
	if (ItemStars == EInv_ItemStar::NoStars) return;		// Empty, continue.
	
	FInv_EnumFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.StarsFragment")));

	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_EnumFragment>(Fragment));
}

void FInv_ItemManifest::TryMakeItemSellValueDisplayFragment()
{
	if (SellValue == 0) return;								// Empty, continue.
	
	FInv_LabeledNumberFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.SellValueFragment")));
	Fragment.LabelColor = EInv_Colors::UIWhite;
	Fragment.ValueColor = EInv_Colors::LegendaryYellow;
	Fragment.SetLabelText(FText::FromString(TEXT("Sell Value: ")));
	Fragment.SetLabelValue(SellValue);

	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_LabeledNumberFragment>(Fragment));
}

void FInv_ItemManifest::TryMakeItemIconDisplayFragment()
{
	if (ItemIcon == nullptr) return;
	
	FInv_ImageFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.IconFragment")));

	Fragment.SetIcon(ItemIcon.Get());
	Fragment.SetIconDimensions(GetIconDimensionsFromRatio(ItemSize));

	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_ImageFragment>(Fragment));
}

FVector2D FInv_ItemManifest::GetIconDimensionsFromRatio(FIntPoint GridSize)
{
	int32 X = FMath::Max(GridSize.X, 1);
	int32 Y = FMath::Max(GridSize.Y, 1);

	const float BaseSize = 64.0f;

	// If perfectly square, lock it to base size
	if (X == Y)
	{
		return FVector2D(BaseSize, BaseSize);
	}

	// For non-square items, check if aspect ratio is reasonable (like 3:2)
	float AspectRatio = static_cast<float>(FMath::Max(X, Y)) / static_cast<float>(FMath::Min(X, Y));
    
	// If aspect ratio is reasonable (not too extreme), allow larger sizes
	if (AspectRatio <= 2.0f) // Adjust this threshold as needed (2.0 allows up to 2:1 ratios)
	{
		const float MaxSize = 96.0f;
        
		if (X > Y) // wider than tall
		{
			return FVector2D(MaxSize, MaxSize * static_cast<float>(Y) / static_cast<float>(X));
		}
		else // taller than wide
		{
			return FVector2D(MaxSize * static_cast<float>(X) / static_cast<float>(Y), MaxSize);
		}
	}
	else
	{
		// For extreme aspect ratios, keep at base size to maintain readability
		if (X > Y) // wider than tall
		{
			return FVector2D(BaseSize, BaseSize * static_cast<float>(Y) / static_cast<float>(X));
		}
		else // taller than wide
		{
			return FVector2D(BaseSize * static_cast<float>(X) / static_cast<float>(Y), BaseSize);
		}
	}
}


void FInv_ItemManifest::TryMakeItemGridDisplayFragment()
{
	FInv_GridFragment Fragment;
	Fragment.SetFragmentTag(FGameplayTag::RequestGameplayTag(FName("FragmentTags.GridFragment")));
	Fragment.SetGridSize(ItemSize);
	
	DisplayFragments.Add(TInstancedStruct<FInv_ItemFragment>::Make<FInv_GridFragment>(Fragment));
}

void FInv_ItemManifest::ForEachFragmentSetOwningManifest(FInv_ItemManifest* InManifest)
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : DisplayFragments)
	{
		if (FInv_ItemFragment* BasePtr = Fragment.GetMutablePtr<FInv_ItemFragment>())
		{
			BasePtr->SetOwningManifest(InManifest);
		}
	}
}

void FInv_ItemManifest::AssimilateInventoryFragments(UInv_CompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FInv_InventoryItemFragment>();
	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](UInv_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}

void FInv_ItemManifest::SpawnPickupActor(
	const UObject* WorldContextObject,
	const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	if (!IsValid(PickupActorClass) || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(SpawnedActor)) return;

	// Set the item manifest, item category, item type, etc.
	UInv_ItemComponent* ItemComp = SpawnedActor->FindComponentByClass<UInv_ItemComponent>();
	check(ItemComp);

	ItemComp->InitItemManifest(*this);
}

void FInv_ItemManifest::InitializeFromDataAsset() {
    if (SourceItemDataAsset && SourceItemDataAsset->Entries.IsValidIndex(EntryIndex))
    {
        const FInv_ItemEntry& Entry = SourceItemDataAsset->Entries[EntryIndex];

    	ItemIcon = Entry.ItemIcon;
    	ItemSize = Entry.ItemSize;
        ItemName = Entry.ItemName;
        ItemDescription = Entry.ItemDescription;
    	ItemStars = Entry.ItemStars;
        ItemAttributesContainer = Entry.ItemAttributesContainer;
        ItemAbilitiesContainer = Entry.ItemAbilitiesContainer;
        ItemRarity = Entry.ItemRarity;
        ItemCategory = Entry.ItemCategory;
        ItemType = Entry.ItemType;
        SellValue = Entry.SellValue;
        ExtraFragments = Entry.ExtraFragments;
        PickupActorClass = Entry.PickupActorClass;
    }
}
#pragma endregion
/*-------------------------------------------------------------------------*/
