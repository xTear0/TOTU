// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Inv_StorageGrid.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class UInv_StorageComponent;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Class Functionality                                                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_StorageGrid.h_Class
UCLASS()
class INVENTORY_API UInv_StorageGrid : public UInv_InventoryGrid
{
    GENERATED_BODY()
    
public:
    virtual void NativeOnInitialized() override;
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetStorageComponent(UInv_StorageComponent* Component);
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetCurrentPage(int32 PageIndex);
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void RefreshPage();
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetStorageType(EInv_ItemCategory Category);
    
    // Override parent function
    virtual void AddItem(UInv_InventoryItem* Item) override;
    
protected:
    UFUNCTION()
    void OnStorageItemAdded(UInv_InventoryItem* Item);
    
    UFUNCTION()
    void OnStorageItemRemoved(UInv_InventoryItem* Item);
    
private:
    UPROPERTY()
    TObjectPtr<UInv_StorageComponent> StorageComponent;
    
    int32 CurrentPageIndex = 0;
    EInv_ItemCategory StorageType = EInv_ItemCategory::None;
    
    void LoadPageItems();
    void ClearGrid();
};
#pragma endregion
/*-------------------------------------------------------------------------*/