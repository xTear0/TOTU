// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentManagement/Modifications/Inv_AttributeModificationHandler.h"
#include "TOTUUserWidget.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                         */
/*-------------------------------------------------------------------------*/
class UAttributeHoverSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnAttributeHover,
    bool, bShow,
    FGameplayTag, AttributeTag
);
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Class Functionality                                                   */
/*-------------------------------------------------------------------------*/
#pragma region TOTUUserWidget.h_Class
UCLASS()
class TOTU_API UTOTUUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetWidgetController(UObject* InWidgetController);

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UObject> WidgetController;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attributes")
    FGameplayTag AttributeTag;
    
    // Function for Blueprint to call on mouse enter/leave events
    UFUNCTION(BlueprintCallable, Category = "Attribute Hover")
    void TriggerAttributeHover(bool bShow);
    
    // Direct access to subsystem for Blueprint use
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Attribute Hover")
    UAttributeHoverSubsystem* GetAttributeHoverSubsystem() const;
    
protected:
    UFUNCTION(BlueprintImplementableEvent)
    void WidgetControllerSet();
    
};
#pragma endregion
/*-------------------------------------------------------------------------*/