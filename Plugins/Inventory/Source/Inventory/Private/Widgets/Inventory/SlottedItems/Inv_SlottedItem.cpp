// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Items/Inv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region Inv_SlottedItem.cpp_Functions
FReply UInv_SlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
	OnSlottedItemClick.Broadcast(GridIndex, MouseEvent);
	return FReply::Handled();
}

void UInv_SlottedItem::NativeOnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UInv_InventoryStatics::ItemHovered(GetOwningPlayer(), InventoryItem.Get());
}

void UInv_SlottedItem::NativeOnMouseLeave(const FPointerEvent& MouseEvent)
{
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());	
}

FVector2D UInv_SlottedItem::GetImageSize()
{
	return Image_Icon->GetBrush().GetImageSize();
}

void UInv_SlottedItem::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
	SetItemData(
		Item->GetItemManifestMutable().GetItemRarity(),
		Item->GetItemManifestMutable().GetItemSpecialType(),
		Item->GetItemManifestMutable().GetItemEnhancement()
		);
}

void UInv_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void UInv_SlottedItem::UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
		Text_StackCount->SetText(FText::AsNumber(StackCount));
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInv_SlottedItem::SetItemData(EInv_ItemRarity Rarity, EInv_ItemSpecialType Type, EInv_ItemEnhancement Enhancement)
{
	ItemData.SpecialType = Type;
	ItemData.Enhancement = Enhancement;
	ItemData.Rarity = Rarity;
}
#pragma endregion
/*-------------------------------------------------------------------------*/