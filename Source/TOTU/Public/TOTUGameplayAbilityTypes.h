// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "GameplayEffectTypes.h"
#include "TOTUGameplayAbilityTypes.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FTOTUGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool isDodgedHit() const { return bIsDodgedHit; }
	bool isCriticalHit() const { return bIsCriticalHit; }

	void bSetIsDodgedHit(bool bInIsDodgedHit) { bIsDodgedHit = bInIsDodgedHit; }
	void bSetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FTOTUGameplayEffectContext* Duplicate() const
	{
		FTOTUGameplayEffectContext* NewContext = new FTOTUGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsDodgedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Templates                                                              */
/*-------------------------------------------------------------------------*/
template<>
struct TStructOpsTypeTraits<FTOTUGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTOTUGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
/*-------------------------------------------------------------------------*/