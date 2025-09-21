// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "TOTUGameplayTags.h"
#include "Player/HeroPlayerState.h"
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

	AHeroPlayerState* TOTUPlayerState = CastChecked<AHeroPlayerState>(PlayerState);
	TOTUPlayerState->OnAttributePointChangedDelegate.AddLambda([this](int32 Points)
	{
		AttributePointsChangedDelegate.Broadcast(Points);
	});
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UTOTUAttributeSet* AS = CastChecked<UTOTUAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	AHeroPlayerState* TOTUPlayerState = CastChecked<AHeroPlayerState>(PlayerState);
	AttributePointsChangedDelegate.Broadcast(TOTUPlayerState->GetAttributePoints());
	
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UTOTUAbilitySystemComponent* TOTUASC = CastChecked<UTOTUAbilitySystemComponent>(AbilitySystemComponent);
	TOTUASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FTOTUAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
#pragma endregion
/*-------------------------------------------------------------------------*/