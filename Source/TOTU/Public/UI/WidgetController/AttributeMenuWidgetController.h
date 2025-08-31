// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "UI/WidgetController/TOTUWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UAttributeInfo;
struct FTOTUAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FTOTUAttributeInfo&, Info);
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Class Functionality                                                    */
/*-------------------------------------------------------------------------*/
#pragma region AttributeMenuWidgetController.h_Class
UCLASS(BlueprintType, Blueprintable)
class TOTU_API UAttributeMenuWidgetController : public UTOTUWidgetController
{
	GENERATED_BODY()
public:	
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
#pragma endregion
/*-------------------------------------------------------------------------*/