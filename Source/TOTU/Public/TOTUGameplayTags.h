// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"
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
/*  TOTU Gameplay Tags                                                     */
/*-------------------------------------------------------------------------*/
#pragma region TOTUGameplayTags.h_Class
// Singleton containing native Gameplay Tags
struct FTOTUGameplayTags
{
public:
	static const FTOTUGameplayTags& Get() 
	{ 
		static FTOTUGameplayTags Instance;
		static bool bInitialized = false;
		if (!bInitialized)
		{
			Instance.InitializeNativeGameplayTags();
			bInitialized = true;
		}
		return Instance;
	}
void InitializeNativeGameplayTags()
{
/*------------------------------------------------------------------*/
/*  Primary Attribute Tags                                          */
/*------------------------------------------------------------------*/		
		Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Primary.Strength"),
		FString("Increases physical damage and pierce."));

		Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Primary.Intelligence"),
		FString("Increases maximum mana and mana regeneration."));

		Attributes_Primary_Fortitude = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Primary.Fortitude"),
		FString("Increases dexterity and various resistances."));

		Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Primary.Vitality"),
		FString("Increases maximum health and health regeneration."));
/*------------------------------------------------------------------*/
/*  Secondary Attribute Tags                                        */
/*------------------------------------------------------------------*/
		Attributes_Secondary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.Dexterity"),
		FString("Increases dodge chance and movement speed."));

		Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.MaxMana"),
		FString("Increases maximum allowable mana."));

		Attributes_Secondary_ManaRegen = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.ManaRegen"),
		FString("Constant passive regeneration of mana."));

		Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.MaxHealth"),
		FString("Increases maximum allowable health."));

		Attributes_Secondary_HealthRegen = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.HealthRegen"),
		FString("Constant passive regeneration of health."));

		Attributes_Secondary_CritChance = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.CritChance"),
		FString("The chance of dealing +Crit Damage% more damage."));

		Attributes_Secondary_CritDamage = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.CritDamage"),
		FString("The amount of extra damage a critical hit will deal."));

		Attributes_Secondary_IgnoreDEF = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Secondary.IgnoreDEF"),
		FString("The defense that your attacks will ignore."));
/*------------------------------------------------------------------*/
/*   Input Tags                                                     */
/*------------------------------------------------------------------*/
		InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button."));

		InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button."));

		InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.0"),
		FString("Input Tag for 0 key."));

		InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.1"),
		FString("Input Tag for 1 key."));

		InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.2"),
		FString("Input Tag for 2 key."));

		InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.3"),
		FString("Input Tag for 3 key."));

		InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.4"),
		FString("Input Tag for 4 key."));

		InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.5"),
		FString("Input Tag for 5 key."));

		InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.6"),
		FString("Input Tag for 6 key."));

		InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.7"),
		FString("Input Tag for 7 key."));

		InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.8"),
		FString("Input Tag for 8 key."));

		InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("InputTag.9"),
		FString("Input Tag for 9 key."));
/*------------------------------------------------------------------*/
/*  Primary Attribute Tags                                          */
/*------------------------------------------------------------------*/		
		Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute"));
/*------------------------------------------------------------------*/
/*   Damage Resistances                                             */
/*------------------------------------------------------------------*/
		Attributes_Resistance = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance"),
		FString("Global Resistance Type"));

		Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Physical"),
		FString("Physical Damage Resistance Type"));

		Attributes_Resistance_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Poison"),
		FString("Poison Damage Resistance Type"));

		Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Ice"),
		FString("Ice Damage Resistance Type"));

		Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Fire"),
		FString("Fire Damage Resistance Type"));

		Attributes_Resistance_Light = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Light"),
		FString("Light Damage Resistance Type"));

		Attributes_Resistance_Shadow = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Attributes.Resistance.Shadow"),
		FString("Shadow Damage Resistance Type"));
/*------------------------------------------------------------------*/
/*   Damage Types                                                   */
/*------------------------------------------------------------------*/
		Damage = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage"),
		FString("Damage"));

		Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Physical"),
		FString("Physical Damage Type"));

		Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Poison"),
		FString("Poison Damage Type"));

		Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Ice"),
		FString("Ice Damage Type"));

		Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Fire"),
		FString("Fire Damage Type"));

		Damage_Light = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Light"),
		FString("Light Damage Type"));

		Damage_Shadow = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Damage.Shadow"),
		FString("Shadow Damage Type"));

/*------------------------------------------------------------------*/
/*   Map of Damage Types to Resistances                             */
/*------------------------------------------------------------------*/
		DamageTypesToResistances.Add(Damage_Physical, Attributes_Resistance_Physical);
		DamageTypesToResistances.Add(Damage_Poison, Attributes_Resistance_Poison);
		DamageTypesToResistances.Add(Damage_Ice, Attributes_Resistance_Ice);
		DamageTypesToResistances.Add(Damage_Fire, Attributes_Resistance_Fire);
		DamageTypesToResistances.Add(Damage_Light, Attributes_Resistance_Light);
		DamageTypesToResistances.Add(Damage_Shadow, Attributes_Resistance_Shadow);
/*------------------------------------------------------------------*/
/*   Effects                                                        */
/*------------------------------------------------------------------*/
		Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Effects.HitReact"),
		FString("Tag Granted when Hit Reacting"));
/*------------------------------------------------------------------*/
/*   Abilities                                                      */
/*------------------------------------------------------------------*/
		Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Abilities.Attack"),
		FString("Attack Ability Tag"));

		Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag"));


/*------------------------------------------------------------------*/
/*   Cooldowns                                                      */
/*------------------------------------------------------------------*/
		Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag"));
	
/*------------------------------------------------------------------*/
/*   Sockets                                                       */
/*------------------------------------------------------------------*/
		CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("CombatSocket.Weapon"),
		FString("Weapon"));
		CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("CombatSocket.RightHand"),
		FString("Right Hand"));
		CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("CombatSocket.LeftHand"),
		FString("Left Hand"));

/*------------------------------------------------------------------*/
/*   Montages                                                       */
/*------------------------------------------------------------------*/
		Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Montage.Attack.1"),
	FString("Attack 1"));
		Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Montage.Attack.2"),
	FString("Attack 2"));
		Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Montage.Attack.3"),
	FString("Attack 3"));
		Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Montage.Attack.4"),
	FString("Attack 4"));
		Montage_Attack_5 = UGameplayTagsManager::Get().AddNativeGameplayTag
		(FName("Montage.Attack.5"),
	FString("Attack 5"));
}
	
	/* Primary Attribute Tags */
	// Base attributes scaled by Class Level and curve tables.
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Fortitude;
	FGameplayTag Attributes_Primary_Vitality;
	

	/* Secondary Attribute Tags */
	// Attributes derrived from the Primary Attributes and modified by curve tables.
	FGameplayTag Attributes_Secondary_Dexterity;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_CritChance;
	FGameplayTag Attributes_Secondary_CritDamage;
	FGameplayTag Attributes_Secondary_IgnoreDEF;

	/* Meta Attribute Tags */
	FGameplayTag Attributes_Meta_IncomingXP;
	
	
	// Input Tags
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_0;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_5;
	FGameplayTag InputTag_6;
	FGameplayTag InputTag_7;
	FGameplayTag InputTag_8;
	FGameplayTag InputTag_9;

	/* Damage Resistance Tags */
	FGameplayTag Attributes_Resistance;
	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Poison;
	FGameplayTag Attributes_Resistance_Ice;
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Light;
	FGameplayTag Attributes_Resistance_Shadow;
	
	/* Damage Type Tags */
	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Poison;
	FGameplayTag Damage_Ice;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Light;
	FGameplayTag Damage_Shadow;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;


	/* Ability Tags */
	FGameplayTag Abilities_Attack;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_FireBolt;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Attack_5;

	FGameplayTag Effects_HitReact;

private:
	static FTOTUGameplayTags GameplayTags;
};
#pragma endregion
/*-------------------------------------------------------------------------*/