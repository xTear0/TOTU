// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/HeroPlayerState.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region OverlayWidgetController.cpp_Functions
void UOverlayWidgetController::BroadcastInitialValues()
{
	const UTOTUAttributeSet* TOTUAttributeSet = CastChecked<UTOTUAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TOTUAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TOTUAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(TOTUAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(TOTUAttributeSet->GetMaxMana());
}
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AHeroPlayerState* TOTUPlayerState = CastChecked<AHeroPlayerState>(PlayerState);
	TOTUPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	TOTUPlayerState->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	
	const UTOTUAttributeSet* TOTUAttributeSet = CastChecked<UTOTUAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TOTUAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TOTUAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TOTUAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TOTUAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (UTOTUAbilitySystemComponent* TOTUASC = Cast<UTOTUAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (TOTUASC->bStartupAbilitieGiven)
		{
			OnInitializeStartupAbilities(TOTUASC);
		}
		else
		{
			TOTUASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
	
		TOTUASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{	
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UTOTUAbilitySystemComponent* TOTUAbilitySystemComponent)
{
	if (!TOTUAbilitySystemComponent->bStartupAbilitieGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, TOTUAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FTOTUAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TOTUAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = TOTUAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	TOTUAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AHeroPlayerState* TOTUPlayerState = CastChecked<AHeroPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = TOTUPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out HeroPlayerState Blueprint"));

	int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		
		const float XPBarPercentage = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercentage);
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/