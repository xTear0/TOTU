// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/Widget/TOTUUserWidget.h"
#include "Widgets/Subsystems/AttributeHoverSubsystem.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region TOTUUserWidget.cpp_Functions

void UTOTUUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

void UTOTUUserWidget::TriggerAttributeHover(bool bShow)
{
	if (UAttributeHoverSubsystem* HoverSubsystem = GetAttributeHoverSubsystem())
	{
		HoverSubsystem->BroadcastAttributeHover(bShow, AttributeTag);
	}
}

UAttributeHoverSubsystem* UTOTUUserWidget::GetAttributeHoverSubsystem() const
{
	return UAttributeHoverSubsystem::Get(this);
}

#pragma endregion
/*-------------------------------------------------------------------------*/