// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Inv_WidgetUtils.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality									                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_WidgetUtils.h_Class
struct FGameplayTag;
enum class EInv_ItemAttribute : uint8;

UCLASS()
class INVENTORY_API UInv_WidgetUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FVector2D GetWidgetPosition(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FVector2D GetWidgetSize(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FLinearColor GetColorFromColorEnum(EInv_Colors RequestedColor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FLinearColor GetColorFromRarityEnum(EInv_ItemRarity RequestedRarity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FLinearColor GetColorFromAttributeTag(FGameplayTag RequestedAttributeTag);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FText GetPrefixFromRarityEnum(EInv_ItemRarity RequestedRarity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FText GetItemTypeNameFromTag(FGameplayTag ItemType);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FText GetAttributeTextLabel(FGameplayTag RequestedAttributeTag);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FText GetAttributeTextName(FGameplayTag RequestedAttributeTag);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static FText GetSignedAttributeValueFromInt(int32 RequestedValue);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static bool IsWithinWidgetBounds(const FVector2D& WidgetPos, const FVector2D& WidgetSize, const FVector2D& MousePos);
	
	static FVector2D GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePosition);
	static int32 GetIndexFromPosition(const FIntPoint& Position, const int32 Columns);
	static FIntPoint GetPositionFromIndex(const int32 Index, const int32 Columns);
};
#pragma endregion
/*-------------------------------------------------------------------------*/