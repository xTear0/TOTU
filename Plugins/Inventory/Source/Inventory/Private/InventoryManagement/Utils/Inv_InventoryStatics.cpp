// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "InventoryManagement/Components/Inv_StorageComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Types/Inv_EnumTypes.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region Inv_InventoryStatics.cpp_Functions
UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;
}

EInv_ItemCategory UInv_InventoryStatics::GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComp)
{
	if (!IsValid(ItemComp)) return EInv_ItemCategory::None;
	return ItemComp->GetItemManifest().GetItemCategory();
}

void UInv_InventoryStatics::ItemHovered(APlayerController* PC, UInv_InventoryItem* Item)
{
	UInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	UInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;
	
	if (InventoryBase->HasHoverItem()) return;
	InventoryBase->OnItemHovered(Item);
}

void UInv_InventoryStatics::ItemUnhovered(APlayerController* PC)
{
	UInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	UInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	InventoryBase->OnItemUnhovered();
}

UInv_HoverItem* UInv_InventoryStatics::GetHoverItem(APlayerController* PC)
{
	UInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	UInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return nullptr;

	return InventoryBase->GetHoverItem();
}

UInv_InventoryBase* UInv_InventoryStatics::GetInventoryWidget(APlayerController* PC)
{
	UInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	return IC->GetInventoryMenu();
}

UInv_StorageComponent* UInv_InventoryStatics::GetStorageComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	return PlayerController->FindComponentByClass<UInv_StorageComponent>();
}

bool UInv_InventoryStatics::TransferItemToStorage(APlayerController* PC, UInv_InventoryItem* Item, int32 PageIndex)
{
	if (!IsValid(PC) || !IsValid(Item)) return false;
    
	UInv_StorageComponent* StorageComp = GetStorageComponent(PC);
	UInv_InventoryComponent* InventoryComp = GetInventoryComponent(PC);
    
	if (!IsValid(StorageComp) || !IsValid(InventoryComp)) return false;
    
	StorageComp->Server_TransferToStorage(Item, PageIndex, Item->GetTotalStackCount());
	return true;
}

bool UInv_InventoryStatics::TransferItemFromStorage(APlayerController* PC, UInv_InventoryItem* Item)
{
	if (!IsValid(PC) || !IsValid(Item)) return false;
    
	UInv_StorageComponent* StorageComp = GetStorageComponent(PC);
	if (!IsValid(StorageComp)) return false;
    
	StorageComp->Server_TransferToInventory(Item, Item->GetTotalStackCount());
	return true;
}
#pragma endregion
/*-------------------------------------------------------------------------*/

