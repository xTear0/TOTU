// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "TOTUGameplayTags.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region AttributeMenuWidgetController.cpp_Functions
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UTOTUAttributeSet* AS = CastChecked<UTOTUAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UTOTUAttributeSet* AS = CastChecked<UTOTUAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

}
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FTOTUAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
#pragma endregion
/*-------------------------------------------------------------------------*/