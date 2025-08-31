// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TOTUAttributeSet.generated.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};
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
//		DA_AttributeInfo
//
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Attribute Construction and Blueprint Exposure                         */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAttributeSet.h_Class
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class TOTU_API UTOTUAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTOTUAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

/*-------------------------------------------------------------------------*/
#pragma region FGameplay_Attributes
/*-------------------------------------------------------------------*/
/*   Meta Attributes								                 */
/*-------------------------------------------------------------------*/

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, IncomingXP);

/*-------------------------------------------------------------------*/
/*   Vital Attributes								                 */
/*-------------------------------------------------------------------*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Mana);

/*-------------------------------------------------------------------*/
/*   Primary Attributes								                 */
/*-------------------------------------------------------------------*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vitality, Category = "Primary Attributes")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Vitality);

/*-------------------------------------------------------------------*/
/*   Secondary Attributes								             */
/*-------------------------------------------------------------------*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Secondary Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Secondary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Dexterity);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, MaxMana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, HealthRegen);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritChance, Category = "Secondary Attributes")
	FGameplayAttributeData CritChance;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, CritChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, CritDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IgnoreDEF, Category = "Secondary Attributes")
	FGameplayAttributeData IgnoreDEF;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, IgnoreDEF);

/*-------------------------------------------------------------------*/
/*   Resistance Attributes								             */
/*-------------------------------------------------------------------*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Resistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, PhysicalRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PoisonResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PoisonRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, PoisonRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IceResistance, Category = "Resistance Attributes")
	FGameplayAttributeData IceRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, IceRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Resistance Attributes")
	FGameplayAttributeData FireRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, FireRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightResistance, Category = "Resistance Attributes")
	FGameplayAttributeData LightRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, LightRes);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShadowResistance, Category = "Resistance Attributes")
	FGameplayAttributeData ShadowRes;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, ShadowRes);

/*-------------------------------------------------------------------*/
/*   Deprecated Attributes (Unused until further notice)			 */
/*-------------------------------------------------------------------*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Pierce, Category = "Secondary Attributes")
	FGameplayAttributeData Pierce;
	ATTRIBUTE_ACCESSORS(UTOTUAttributeSet, Pierce);

/*-------------------------------------------------------------------*/
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
#pragma region OnRep_AttributeFunctions
/*-------------------------------------------------------------------*/
/*   Vital Attribute Functions                                       */
/*-------------------------------------------------------------------*/
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

/*-------------------------------------------------------------------*/
/*   Primary Attribute Functions                                     */
/*-------------------------------------------------------------------*/

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;

/*-------------------------------------------------------------------*/
/*   Secondary Attribute Functions                                   */
/*-------------------------------------------------------------------*/	
	
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	UFUNCTION()
	void OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const;
	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const;
	UFUNCTION()
	void OnRep_IgnoreDEF(const FGameplayAttributeData& OldIgnoreDEF) const;

/*-------------------------------------------------------------------*/
/*   Resistance Attribute Functions                                  */
/*-------------------------------------------------------------------*/
	UFUNCTION()
	void OnRep_Resistance(const FGameplayAttributeData& OldResistance) const;
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalRes) const;
	UFUNCTION()
	void OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonRes) const;
	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireRes) const;
	UFUNCTION()
	void OnRep_LightResistance(const FGameplayAttributeData& OldLightRes) const;
	UFUNCTION()
	void OnRep_ShadowResistance(const FGameplayAttributeData& OldShadowRes) const;
/*-------------------------------------------------------------------*/
/*   Deprecated Attribute Functions                                  */
/*-------------------------------------------------------------------*/

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_Pierce(const FGameplayAttributeData& OldPierce) const;

/*-------------------------------------------------------------------*/
#pragma endregion
/*-------------------------------------------------------------------------*/

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties) const;

	void ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bIsDodgedHit, bool bIsCriticalHit) const;

	void SendXPEvent(const FEffectProperties& Properties);
};
#pragma endregion
/*-------------------------------------------------------------------------*/