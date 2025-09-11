// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "InventoryManagement/Components/Inv_StorageComponent.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
UInv_StorageComponent::UInv_StorageComponent() : StorageList(this)
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    bReplicateUsingRegisteredSubObjectList = true;
}

void UInv_StorageComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME_CONDITION(ThisClass, StorageList, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(ThisClass, CurrentPage, COND_OwnerOnly);
}
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region Inv_StorageComponent.cpp_Functions
void UInv_StorageComponent::ToggleStorageMenu()
{
    if (bStorageMenuOpen)
    {
        CloseStorageMenu();
    }
    else
    {
        OpenStorageMenu();
    }
    OnStorageMenuToggled.Broadcast(bStorageMenuOpen);
}

void UInv_StorageComponent::BeginPlay()
{
    Super::BeginPlay();
    OwningController = Cast<APlayerController>(GetOwner());
    ConstructStorageMenu();
}

void UInv_StorageComponent::ConstructStorageMenu()
{
    OwningController = Cast<APlayerController>(GetOwner());
    checkf(OwningController.IsValid(), TEXT("Storage Component should have a Player Controller as Owner."));
    if (!OwningController->IsLocalController()) return;

    StorageMenu = CreateWidget<UInv_InventoryBase>(OwningController.Get(), StorageMenuClass);
    StorageMenu->AddToViewport();
    CloseStorageMenu();
}

void UInv_StorageComponent::OpenStorageMenu()
{
    // Starting checks and visibility.
    if (!IsValid(StorageMenu)) return;
    StorageMenu->SetVisibility(ESlateVisibility::Visible);
    bStorageMenuOpen = true;
    if (!OwningController.IsValid()) return;

    // Player can have mouse access.
    FInputModeGameAndUI InputMode;
    OwningController->SetInputMode(InputMode);
    OwningController->SetShowMouseCursor(true);
}

void UInv_StorageComponent::CloseStorageMenu()
{
    // Starting checks and visibility.
    if (!IsValid(StorageMenu)) return;
    StorageMenu->SetVisibility(ESlateVisibility::Collapsed);
    bStorageMenuOpen = false;
    if (!OwningController.IsValid()) return;
    FInputModeGameOnly InputMode;
    OwningController->SetInputMode(InputMode);
    OwningController->SetShowMouseCursor(false);
}

void UInv_StorageComponent::TryAddItemToStorage(UInv_InventoryItem* Item, int32 PageIndex)
{
    if (!IsValid(Item)) return;
    if (Item->GetItemManifest().GetItemCategory() != StorageItemCategory && StorageItemCategory != EInv_ItemCategory::None) return;
    
    Server_AddItemToStorage(Item, PageIndex, Item->GetTotalStackCount());
}

void UInv_StorageComponent::TryRemoveItemFromStorage(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    Server_RemoveItemFromStorage(Item, Item->GetTotalStackCount());
}

TArray<UInv_InventoryItem*> UInv_StorageComponent::GetItemsOnPage(int32 PageIndex) const
{
    return StorageList.GetItemsOnPage(PageIndex);
}

void UInv_StorageComponent::Server_AddItemToStorage_Implementation(UInv_InventoryItem* Item, int32 PageIndex, int32 StackCount)
{
    if (!IsValid(Item)) return;
    
    UInv_InventoryItem* StorageItem = StorageList.FindFirstItemByTypeOnPage(Item->GetItemManifest().GetItemType(), PageIndex);
    
    if (StorageItem && Item->IsStackable())
    {
        StorageItem->SetTotalStackCount(StorageItem->GetTotalStackCount() + StackCount);
    }
    else
    {
        UInv_InventoryItem* NewItem = StorageList.AddEntry(Item, PageIndex);
        NewItem->SetTotalStackCount(StackCount);
        AddRepSubObj(NewItem);
    }
    
    OnStorageItemAdded.Broadcast(Item);
}

void UInv_StorageComponent::Server_RemoveItemFromStorage_Implementation(UInv_InventoryItem* Item, int32 StackCount)
{
    if (!IsValid(Item)) return;
    
    const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
    if (NewStackCount <= 0)
    {
        StorageList.RemoveEntry(Item);
        OnStorageItemRemoved.Broadcast(Item);
    }
    else
    {
        Item->SetTotalStackCount(NewStackCount);
    }
}

void UInv_StorageComponent::Server_TransferToInventory_Implementation(UInv_InventoryItem* StorageItem, int32 StackCount)
{
    if (!IsValid(StorageItem)) return;
    
    UInv_InventoryComponent* InventoryComp = UInv_InventoryStatics::GetInventoryComponent(OwningController.Get());
    if (!IsValid(InventoryComp)) return;
    
    // Create a temporary item component for transfer
    UInv_ItemComponent* TempItemComp = NewObject<UInv_ItemComponent>(this);
    TempItemComp->InitItemManifest(StorageItem->GetItemManifest());
    
    InventoryComp->TryAddItem(TempItemComp);
    Server_RemoveItemFromStorage(StorageItem, StackCount);
}

void UInv_StorageComponent::Server_TransferToStorage_Implementation(UInv_InventoryItem* InventoryItem, int32 PageIndex, int32 StackCount)
{
    if (!IsValid(InventoryItem)) return;
    
    Server_AddItemToStorage(InventoryItem, PageIndex, StackCount);
    
    UInv_InventoryComponent* InventoryComp = UInv_InventoryStatics::GetInventoryComponent(OwningController.Get());
    if (IsValid(InventoryComp))
    {
        InventoryComp->Server_DropItem(InventoryItem, StackCount);
    }
}

void UInv_StorageComponent::AddRepSubObj(UObject* SubObj)
{
    if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
    {
        AddReplicatedSubObject(SubObj);
    }
}

/*-------------------------------------------------------------------------*/
/*   Fast Array Implementation                                             */
/*-------------------------------------------------------------------------*/
TArray<UInv_InventoryItem*> FInv_StorageFastArray::GetItemsOnPage(int32 PageIndex) const
{
    TArray<UInv_InventoryItem*> Results;
    for (const auto& Entry : Entries)
    {
        if (Entry.PageIndex == PageIndex && IsValid(Entry.Item))
        {
            Results.Add(Entry.Item);
        }
    }
    return Results;
}

UInv_InventoryItem* FInv_StorageFastArray::AddEntry(UInv_ItemComponent* ItemComponent, int32 PageIndex)
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    check(OwningActor->HasAuthority());
    
    FInv_StorageEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor, ItemComponent);
    NewEntry.PageIndex = PageIndex;
    
    MarkItemDirty(NewEntry);
    return NewEntry.Item;
}

UInv_InventoryItem* FInv_StorageFastArray::AddEntry(UInv_InventoryItem* Item, int32 PageIndex)
{
    check(OwnerComponent);
    AActor* OwningActor = OwnerComponent->GetOwner();
    check(OwningActor->HasAuthority());
    
    FInv_StorageEntry& NewEntry = Entries.AddDefaulted_GetRef();
    NewEntry.Item = Item;
    NewEntry.PageIndex = PageIndex;
    
    MarkItemDirty(NewEntry);
    return Item;
}

void FInv_StorageFastArray::RemoveEntry(UInv_InventoryItem* Item)
{
    for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
    {
        FInv_StorageEntry& Entry = *EntryIt;
        if (Entry.Item == Item)
        {
            EntryIt.RemoveCurrent();
            MarkArrayDirty();
            break;
        }
    }
}

UInv_InventoryItem* FInv_StorageFastArray::FindFirstItemByTypeOnPage(const FGameplayTag& ItemType, int32 PageIndex)
{
    auto* FoundItem = Entries.FindByPredicate([ItemType, PageIndex](const FInv_StorageEntry& Entry)
    {
        return Entry.PageIndex == PageIndex && 
               IsValid(Entry.Item) && 
               Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
    });
    return FoundItem ? FoundItem->Item : nullptr;
}

void FInv_StorageFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    UInv_StorageComponent* SC = Cast<UInv_StorageComponent>(OwnerComponent);
    if (!IsValid(SC)) return;
    
    for (int32 Index : RemovedIndices)
    {
        SC->OnStorageItemRemoved.Broadcast(Entries[Index].Item);
    }
}

void FInv_StorageFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    UInv_StorageComponent* SC = Cast<UInv_StorageComponent>(OwnerComponent);
    if (!IsValid(SC)) return;
    
    for (int32 Index : AddedIndices)
    {
        SC->OnStorageItemAdded.Broadcast(Entries[Index].Item);
    }
}
#pragma endregion
/*-------------------------------------------------------------------------*/