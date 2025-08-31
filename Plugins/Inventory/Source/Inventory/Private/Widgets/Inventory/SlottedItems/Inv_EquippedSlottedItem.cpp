// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions			                                                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_EquippedSlottedItem.cpp_Functions



#pragma endregion
/*-------------------------------------------------------------------------*/
FReply UInv_EquippedSlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnEquippedSlottedItemClicked.Broadcast(this);
	return FReply::Handled();
}
