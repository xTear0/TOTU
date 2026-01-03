// Copyright xTear Studios
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Inv_AttributeSetAccessor.h"
#include "TOTUAttributeSetAccessor.generated.h"

UCLASS()
class TOTU_API UTOTUAttributeSetAccessor : public UInv_AttributeSetAccessor
{
	GENERATED_BODY()

public:
	virtual FGameplayAttribute GetAttributeFromTag_Implementation(const FGameplayTag& AttributeTag) const override;
};