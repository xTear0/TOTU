// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "Interaction/CombatInterface.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Functions                                                              */
/*-------------------------------------------------------------------------*/
#pragma region MMC_MaxHealth.cpp_Functions
UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UTOTUAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Vitality = 0.f;

	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParams, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 100.f + ( 4.f * Vitality ) + ( 10.f * PlayerLevel );
}
#pragma endregion
/*-------------------------------------------------------------------------*/