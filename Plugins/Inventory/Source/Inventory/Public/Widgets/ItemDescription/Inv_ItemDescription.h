// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Types/Inv_EnumTypes.h"
#include "Widgets/Composite/Inv_Composite.h"
#include "Inv_ItemDescription.generated.h"
/*-------------------------------------------------------------------------*/


/* The Item Description widget that shows up when hovering over an item. */


/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class USizeBox;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality									                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_ItemDescription.h_Class
UCLASS()
class INVENTORY_API UInv_ItemDescription : public UInv_Composite
{
	GENERATED_BODY()

public:

	FVector2D GetBoxSize() const;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	EInv_ItemRarity& GetRarity() { return ItemRarity; }
	
	void SetRarity(const EInv_ItemRarity& Rarity) {  ItemRarity = Rarity; }
	
private:

	UPROPERTY()
	EInv_ItemRarity ItemRarity{EInv_ItemRarity::Common}; 
	
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
#pragma endregion
/*-------------------------------------------------------------------------*/