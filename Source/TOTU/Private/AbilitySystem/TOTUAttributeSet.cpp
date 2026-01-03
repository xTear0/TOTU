// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/TOTUAttributeSet.h"
#include "AbilitySystem/TOTUAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TOTUGameplayTags.h"
#include "Interaction/PlayerInterface.h"
#include "Player/HeroPlayerController.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*					PROCESS FOR ADDING NEW ATTRIBUTES					   */
/*-------------------------------------------------------------------------*/
#pragma region New_Attribute_Manual
//		
//		Step (1):
//		In the Attribute Set Header File, start by creating the FGameplay 
//		Attribute Data, with ATTRIBUTE as the name of your attribute:
//			UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ATTRIBUTE, Category = "Attributes")
//			FGameplayAttributeData ATTRIBUTE;
//	 		ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, ATTRIBUTE);
//	
//		Step (2):
//		In the Attribute Set Header File, add the OnRep function for
//		the new attribute:
//			UFUNCTION()
//			void OnRep_ATTRIBUTE(const FGameplayAttributeData& OldATTRIBUTE) const;
//			
//		Step (3):
//		In the Attribute Set CPP File, add the attribute replication settings for the
//		new attribute:
//			DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, ATTRIBUTE, COND_None, REPNOTIFY_Always);
// 
//		Step (4):
//		In the Attribute Set CPP File, add the attribute replication notification for
//		the new attribute:
//			void UTOTUAttributeSet::OnRep_ATTRIBUTE(const FGameplayAttributeData& OldATTRIBUTE) const
//			{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, ATTRIBUTE, OldATTRIBUTE); }
// 
//		Step (5):
//		In the Attribute Set CPP File, add the Tag for the new attribute.
// 			TagsToAttributes.Add(GameplayTags.Attributes_(PRIORITY)_ATTRIBUTE, GetATTRIBUTEAttribute);
// 
//		Step (6):
//		In the GameplayTags Header File, add the Tag for the new attribute.
//			FGameplayTag Attributes_(PRIORITY)_ATTRIBUTE;
// 
//		Step (7):
//		In the GameplayTags CPP File, add the Tag for the new attribute.
//			GameplayTags.Attributes_(PRIORITY)_ATTRIBUTE = UGameplayTagsManager::Get().AddNativeGameplayTag
//				(FName("Attributes.(PRIORITY).ATTRIBUTE"),
//				FString("Does ATTRIBUTE things."));
//
//		Step (8):
//		Check Blueprint Dependancies. I.e. Curve Tables, Data Tables, Message Tables, etc.
//
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Tag Construction                                                      */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAttributeSet.cpp_Constructor
UTOTUAttributeSet::UTOTUAttributeSet()
{
	const FTOTUGameplayTags& GameplayTags = FTOTUGameplayTags::Get();

	// Primary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Fortitude, GetFortitudeAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vitality, GetVitalityAttribute);
	
	// Secondary Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Dexterity, GetDexterityAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegen, GetManaRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegen, GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritChance, GetCritChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritDamage, GetCritDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_IgnoreDEF, GetIgnoreDEFAttribute);

	// Resistance Attributes
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance, GetResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Poison, GetPoisonResAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Ice, GetIceResAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Light, GetLightResAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Shadow, GetShadowResAttribute);
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Attribute Replication Settings                                        */
/*-------------------------------------------------------------------------*/
#pragma region ReplicatedAttributes
void UTOTUAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
/*-------------------------------------------------------------------*/
/*   Vital Attributes								                 */
/*-------------------------------------------------------------------*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Mana, COND_None, REPNOTIFY_Always);
/*-------------------------------------------------------------------*/
/*   Primary Attributes								                 */
/*-------------------------------------------------------------------*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Fortitude, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
/*-------------------------------------------------------------------*/
/*   Secondary Attributes								             */
/*-------------------------------------------------------------------*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, IgnoreDEF, COND_None, REPNOTIFY_Always);
/*-------------------------------------------------------------------*/
/*   Resistance Attributes								             */
/*-------------------------------------------------------------------*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Resistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, PhysicalRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, PoisonRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, IceRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, FireRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, LightRes, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, ShadowRes, COND_None, REPNOTIFY_Always);
/*-------------------------------------------------------------------*/
/*   Deprecated Attributes								             */
/*-------------------------------------------------------------------*/
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTOTUAttributeSet, Pierce, COND_None, REPNOTIFY_Always);
/*-------------------------------------------------------------------*/
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Attribute Operations												   */
/*-------------------------------------------------------------------------*/
#pragma region AttributeOperationFunctions
void UTOTUAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UTOTUAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const
{

	// Source = Source of the Effect, Target = Target of the Effect (owner of this AS)

	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceASC = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();


	if (IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid() && Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Properties.SourceController == nullptr && Properties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Properties.SourceAvatarActor))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}
		if (Properties.SourceController)
		{
			Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);
		Properties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Properties.TargetAvatarActor);
	}
}

void UTOTUAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Properties;
	SetEffectProperties(Data, Properties);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float	LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Properties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
				SendXPEvent(Properties);
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FTOTUGameplayTags::Get().Effects_HitReact);
				Properties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			if (Properties.SourceCharacter != Properties.TargetCharacter)
			{
				if (AHeroPlayerController* PC = Cast<AHeroPlayerController>(UGameplayStatics::GetPlayerController(Properties.SourceCharacter, 0)))
				{

					const bool bDodge = UTOTUAbilitySystemLibrary::IsDodgedHit(Properties.EffectContextHandle);
					const bool bCrit = UTOTUAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle);
					PC->ShowDamageNumber(LocalIncomingDamage, Properties.TargetCharacter, bDodge, bCrit);
				}
			}

			const bool bDodge = UTOTUAbilitySystemLibrary::IsDodgedHit(Properties.EffectContextHandle);
			const bool bCrit = UTOTUAbilitySystemLibrary::IsCriticalHit(Properties.EffectContextHandle);
			ShowFloatingText(Properties, LocalIncomingDamage, bDodge, bCrit);
		}
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);

		// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
		if (Properties.SourceCharacter->Implements<UPlayerInterface>() && Properties.TargetCharacter->Implements<UCombatInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Properties.SourceCharacter);
			const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Properties.SourceCharacter);

			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Properties.SourceCharacter, CurrentXP + LocalIncomingXP);
			const int32 NumLevelUps = NewLevel - CurrentLevel;

			if (NumLevelUps > 0)
			{
				const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Properties.SourceCharacter, CurrentLevel);
				const int32 AbilityPointsReward = IPlayerInterface::Execute_GetAbilityPointsReward(Properties.SourceCharacter, CurrentLevel);
				IPlayerInterface::Execute_AddToPlayerLevel(Properties.SourceCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddToAttributePoints(Properties.SourceCharacter, AttributePointsReward);
				IPlayerInterface::Execute_AddToAbilityPoints(Properties.SourceCharacter, AbilityPointsReward);	

				bTopOffHealth = true;
				bTopOffMana = true;
				
				IPlayerInterface::Execute_LevelUp(Properties.SourceCharacter);
			}
			IPlayerInterface::Execute_AddToXP(Properties.SourceCharacter, LocalIncomingXP);
		}
	}
}

void UTOTUAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
	
}

void UTOTUAttributeSet::SendXPEvent(const FEffectProperties& Properties)
{
	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Properties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Properties.TargetCharacter);
		const int32 XPReward = UTOTUAbilitySystemLibrary::GetXPRewardForClassAndLevel(Properties.TargetCharacter, TargetClass, TargetLevel);

		const FTOTUGameplayTags& GameplayTags = FTOTUGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Properties.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UTOTUAttributeSet::ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bIsDodgedHit, bool bIsCriticalHit) const
{
	if (Properties.SourceCharacter != Properties.TargetCharacter)
	{
		if (AHeroPlayerController* PC = Cast<AHeroPlayerController>(Properties.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Properties.TargetCharacter, bIsDodgedHit, bIsCriticalHit);
			return;
		}
		if (AHeroPlayerController* PC = Cast<AHeroPlayerController>(Properties.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Properties.TargetCharacter, bIsDodgedHit, bIsCriticalHit);
		}
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Attribute Replication Notification									   */
/*-------------------------------------------------------------------------*/
#pragma region ReplicatedAttributeNotification

/*-------------------------------------------------------------------*/
/*   Vital Attributes								                 */
/*-------------------------------------------------------------------*/
#pragma region Vital_Attributes
void UTOTUAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Health, OldHealth); }

void UTOTUAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Mana, OldMana); }
#pragma endregion
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*   Primary Attributes								                 */
/*-------------------------------------------------------------------*/
#pragma region Primary_Attributes
void UTOTUAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{ 	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Strength, OldStrength); }

void UTOTUAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Intelligence, OldIntelligence); }

void UTOTUAttributeSet::OnRep_Fortitude(const FGameplayAttributeData& OldFortitude) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Fortitude, OldFortitude); }

void UTOTUAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Vitality, OldVitality); }
#pragma endregion
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*   Secondary Attributes								             */
/*-------------------------------------------------------------------*/
#pragma region Secondary_Attributes
void UTOTUAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{ 	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Defense, OldDefense); }

void UTOTUAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Dexterity, OldDexterity); }

void UTOTUAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, MaxMana, OldMaxMana); }

void UTOTUAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, ManaRegen, OldManaRegen); }

void UTOTUAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, MaxHealth, OldMaxHealth); }

void UTOTUAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{ 	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, HealthRegen, OldHealthRegen); }

void UTOTUAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, CritChance, OldCritChance); }

void UTOTUAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, CritDamage, OldCritDamage); }

void UTOTUAttributeSet::OnRep_IgnoreDEF(const FGameplayAttributeData& OldIgnoreDEF) const
{ 	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, IgnoreDEF, OldIgnoreDEF); }
#pragma endregion
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*   Resistance Attributes								             */
/*-------------------------------------------------------------------*/
#pragma region Resistance_Attributes
void UTOTUAttributeSet::OnRep_Resistance(const FGameplayAttributeData& OldResistance) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Resistance, OldResistance); }

void UTOTUAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, PhysicalRes, OldPhysicalRes); }

void UTOTUAttributeSet::OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, PoisonRes, OldPoisonRes); }

void UTOTUAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, IceRes, OldIceRes); }

void UTOTUAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, FireRes, OldFireRes); }

void UTOTUAttributeSet::OnRep_LightResistance(const FGameplayAttributeData& OldLightRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, LightRes, OldLightRes); }

void UTOTUAttributeSet::OnRep_ShadowResistance(const FGameplayAttributeData& OldShadowRes) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, ShadowRes, OldShadowRes); }
#pragma endregion
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*   Deprecated Attributes								             */
/*-------------------------------------------------------------------*/
#pragma region Deprecated_Attributes
void UTOTUAttributeSet::OnRep_Pierce(const FGameplayAttributeData& OldPierce) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Pierce, OldPierce); }

void UTOTUAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{	GAMEPLAYATTRIBUTE_REPNOTIFY(UTOTUAttributeSet, Armor, OldArmor); }
#pragma endregion
/*-------------------------------------------------------------------*/

#pragma endregion
/*-------------------------------------------------------------------------*/