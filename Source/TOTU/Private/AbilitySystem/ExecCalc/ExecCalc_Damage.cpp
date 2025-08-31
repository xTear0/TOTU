// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "AbilitySystem/TOTUAbilitySystemLibrary.h"
#include "TOTUGameplayAbilityTypes.h"
#include "TOTUGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
struct TOTUDamageStatics
{	
	/* Target Attributes to Capture */
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dexterity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PoisonRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightRes);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShadowRes);

	/* Source Attributes to Capture */
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IgnoreDEF);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;	

	TOTUDamageStatics()
	{
		/* Target Attribute Definitions */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, Dexterity, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, Resistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, PhysicalRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, PoisonRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, IceRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, FireRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, LightRes, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, ShadowRes, Target, false);

		/* Source Attributes Definitions */
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, CritChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTOTUAttributeSet, IgnoreDEF, Source, false);

		const FTOTUGameplayTags& Tags = FTOTUGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Dexterity, DexterityDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance, ResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Poison, PoisonResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Ice, IceResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Light, LightResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Shadow, ShadowResDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CritChance, CritChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CritDamage, CritDamageDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_IgnoreDEF, IgnoreDEFDef);
	}
};

static const TOTUDamageStatics& DamageStatics()
{
	static TOTUDamageStatics DStatics;
	return DStatics;
}
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region ExecCalc_Damage.cpp_Constructor
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().DexterityDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().IgnoreDEFDef);

	RelevantAttributesToCapture.Add(DamageStatics().ResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResDef);
	RelevantAttributesToCapture.Add(DamageStatics().PoisonResDef);
	RelevantAttributesToCapture.Add(DamageStatics().IceResDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightResDef);
	RelevantAttributesToCapture.Add(DamageStatics().ShadowResDef);
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   ExecCalc Functions                                                    */
/*-------------------------------------------------------------------------*/
#pragma region ExecCalc_Damage.cpp_Functions
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/* Execution Parameters */
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;


	/* Get Damage Set By Caller Magnitude */
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FTOTUGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		checkf(TOTUDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDef doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TOTUDamageStatics().TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;
		Damage += DamageTypeValue;
	}

	// Capture Target Attributes
	float TargetDefense = 0.f; // Default Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParams, TargetDefense);
	TargetDefense = FMath::Max<float>(0.f, TargetDefense);

	float TargetDexterity = 0.f; // Default Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DexterityDef, EvaluationParams, TargetDexterity);
	TargetDexterity = FMath::Max<float>(0.f, TargetDexterity);
	
	// Capture Source Attributes
	float SourceCritChance = 10.f; // Default Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritChanceDef, EvaluationParams, SourceCritChance);
	SourceCritChance = FMath::Max<float>(0.f, SourceCritChance);
	
	float SourceCritDamage = 100.f; // Default Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, EvaluationParams, SourceCritDamage);
	SourceCritDamage = FMath::Max<float>(0.f, SourceCritDamage);

	float SourceIgnoreDEF = 0.f; // Default Value
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IgnoreDEFDef, EvaluationParams, SourceIgnoreDEF);
	SourceIgnoreDEF = FMath::Max<float>(0.f, SourceIgnoreDEF);

	/* Ignore Defense ingores a percentage of the players defense. */
	const float EffectiveDefense = TargetDefense * (100 - SourceIgnoreDEF) / 100.f;
	Damage *= (100 - EffectiveDefense) / 100.f;

	/* Critical Hits check for a hit and then deal double damage scaled by the Critical Damage attribute */
	const bool bCriticalHit = FMath::RandRange(1, 100) <= SourceCritChance;
	Damage = bCriticalHit ? Damage = Damage + ( Damage * ( SourceCritDamage / 100 ) ) : Damage;

	/* Dodge chance calculated from Dexterity, but also not fully implemented yet because of Game Design decisions. */
	const float EffectiveDodgeChance = 0.03 + (TargetDexterity / 100); // 3% base dodge + dexterity.

	const bool bDodged = FMath::RandRange(1, 100) <= EffectiveDodgeChance;
	if (!bCriticalHit && bDodged)
	{
		Damage = 0;
	}

	const FGameplayModifierEvaluatedData EvaluatedData(UTOTUAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);

	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	/* Send out this interaction's Gameplay Effect Context. */
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UTOTUAbilitySystemLibrary::SetIsDodgedHit(EffectContextHandle, bDodged);
	UTOTUAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
}
#pragma endregion
/*-------------------------------------------------------------------------*/
