// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Inv_ItemAttributeBreakdownLine.generated.h"
/*-------------------------------------------------------------------------*/



UCLASS()
class INVENTORY_API UInv_ItemAttributeBreakdownLine : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetLineData(
		const FText& AttributeText,
		const FLinearColor& AttributeColor, 
		const FText& ValueText,
		const FLinearColor& ValueColor) const;

	
protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemValue;
};
/*-------------------------------------------------------------------------*/
