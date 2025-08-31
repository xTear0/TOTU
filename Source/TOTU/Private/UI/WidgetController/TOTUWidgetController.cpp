// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/WidgetController/TOTUWidgetController.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region TOTUWidgetController.cpp_Functions
void UTOTUWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTOTUWidgetController::BroadcastInitialValues()
{

}
void UTOTUWidgetController::BindCallbacksToDependencies()
{

}
#pragma endregion
/*-------------------------------------------------------------------------*/