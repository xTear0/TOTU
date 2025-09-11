// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Widgets/Inventory/Spatial/Inv_StorageGrid.h"
#include "InventoryManagement/Components/Inv_StorageComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
void UInv_StorageGrid::NativeOnInitialized()
{
    // Call grandparent initialization to skip inventory binding
    UUserWidget::NativeOnInitialized();
    
    // Only construct the grid, don't bind to inventory component
    ConstructGrid();
    
    // Storage grid should only listen to storage component events
    // which are set up via SetStorageComponent()
}

void UInv_StorageGrid::SetStorageComponent(UInv_StorageComponent* Component)
{
    if (!IsValid(Component)) return;
    
    StorageComponent = Component;
    
    // Bind to storage events
    if (!StorageComponent->OnStorageItemAdded.IsAlreadyBound(this, &ThisClass::OnStorageItemAdded))
    {
        StorageComponent->OnStorageItemAdded.AddDynamic(this, &ThisClass::OnStorageItemAdded);
    }
    
    if (!StorageComponent->OnStorageItemRemoved.IsAlreadyBound(this, &ThisClass::OnStorageItemRemoved))
    {
        StorageComponent->OnStorageItemRemoved.AddDynamic(this, &ThisClass::OnStorageItemRemoved);
    }
}

void UInv_StorageGrid::SetCurrentPage(int32 PageIndex)
{
    CurrentPageIndex = PageIndex;
}

void UInv_StorageGrid::RefreshPage()
{
    ClearGrid();
    LoadPageItems();
}

void UInv_StorageGrid::SetStorageType(EInv_ItemCategory Category)
{
    StorageType = Category;
    ItemCategory = Category; // Set parent's item category
}

void UInv_StorageGrid::AddItem(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    if (!MatchesCategory(Item)) return;
    
    // Find available slot for the item
    FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);
    
    // Add item to grid slots
    for (const auto& Availability : Result.SlotAvailabilities)
    {
        AddItemAtIndex(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
        UpdateGridSlots(Item, Availability.Index, Result.bStackable, Availability.AmountToFill);
    }
}

void UInv_StorageGrid::OnStorageItemAdded(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    
    // Check if this item belongs to the current page
    TArray<UInv_InventoryItem*> PageItems = StorageComponent->GetItemsOnPage(CurrentPageIndex);
    if (PageItems.Contains(Item))
    {
        AddItem(Item);
    }
}

void UInv_StorageGrid::OnStorageItemRemoved(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    
    // Find and remove the item from the grid
    for (int32 i = 0; i < GridSlots.Num(); i++)
    {
        if (GridSlots[i]->GetInventoryItem() == Item)
        {
            RemoveItemFromGrid(Item, i);
            break;
        }
    }
}

void UInv_StorageGrid::LoadPageItems()
{
    if (!IsValid(StorageComponent)) return;
    
    TArray<UInv_InventoryItem*> PageItems = StorageComponent->GetItemsOnPage(CurrentPageIndex);
    
    for (UInv_InventoryItem* Item : PageItems)
    {
        if (IsValid(Item))
        {
            AddItem(Item);
        }
    }
}

void UInv_StorageGrid::ClearGrid()
{
    // Clear visual representation
    for (auto& SlottedItemPair : SlottedItems)
    {
        if (IsValid(SlottedItemPair.Value))
        {
            SlottedItemPair.Value->RemoveFromParent();
        }
    }
    SlottedItems.Empty();
    
    // Reset grid slots
    for (UInv_GridSlot* GridSlot : GridSlots)
    {
        if (IsValid(GridSlot))
        {
            GridSlot->SetInventoryItem(nullptr);
            GridSlot->SetUpperLeftIndex(INDEX_NONE);
            GridSlot->SetUnoccupiedTexture();
            GridSlot->SetIsAvailable(true);
            GridSlot->SetStackCount(0);
        }
    }
}
/*-------------------------------------------------------------------------*/