// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Types/Inv_EnumTypes.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialStorage.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class UInv_InventoryGrid;
class UInv_StorageGrid;
class UButton;
class UCanvasPanel;
class UTextBlock;
class UInv_ItemDescription;
class UInv_StorageComponent;
class UInv_InventoryComponent;
class UInv_HoverItem;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Class Functionality                                                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_SpatialStorage.h_Class
UCLASS()
class INVENTORY_API UInv_SpatialStorage : public UInv_InventoryBase
{
    GENERATED_BODY()
    
public:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent) override;
    
    // Override from base class
    virtual void OnItemHovered(UInv_InventoryItem* Item) override;
    virtual void OnItemUnhovered() override;
    virtual bool HasHoverItem() const override;
    virtual UInv_HoverItem* GetHoverItem() const override;

    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetStorageType(EInv_ItemCategory Category);
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void RefreshStoragePage();
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    int32 GetCurrentPage() const { return CurrentPageIndex; }
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetCurrentPage(int32 PageIndex);
    
protected:
    // Navigation
    UFUNCTION()
    void OnPreviousPageClicked();
    
    UFUNCTION()
    void OnNextPageClicked();
    
    UFUNCTION()
    void OnPageAdded();
    
private:
    // Handle cross-grid item transfer and hovering
    void HandleCrossGridInteraction();
    bool TryTransferHoveredItem();
    UInv_InventoryGrid* GetActiveHoverGrid() const;
    UInv_InventoryGrid* GetTargetGrid(const FVector2D& MousePos) const;
    void UpdateCrossGridHovering();
    
    // UI Components
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInv_StorageGrid> Grid_Storage;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UInv_InventoryGrid> Grid_PlayerInventory;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_PreviousPage;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_NextPage;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button_AddPage;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_PageNumber;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Text_StorageType;
    
    // Item Description
    UPROPERTY(EditAnywhere, Category = "Storage")
    TSubclassOf<UInv_ItemDescription> ItemDescriptionClass;
    
    UPROPERTY()
    TObjectPtr<UInv_ItemDescription> ItemDescription;
    
    // Components
    UPROPERTY()
    TObjectPtr<UInv_StorageComponent> StorageComponent;
    
    UPROPERTY()
    TObjectPtr<UInv_InventoryComponent> InventoryComponent;
    
    // State
    int32 CurrentPageIndex = 0;
    int32 MaxPages = 3;
    EInv_ItemCategory CurrentStorageType = EInv_ItemCategory::Equippable;
    
    // Timer for item description
    FTimerHandle DescriptionTimer;
    
    UPROPERTY(EditAnywhere, Category = "Storage")
    float DescriptionTimerDelay = 0.5f;
    
    // Helper functions
    void UpdatePageDisplay() const;
    void SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const;
    UInv_ItemDescription* GetItemDescription(const EInv_ItemRarity& Rarity = EInv_ItemRarity::Common, bool UseRarity = false);
    void InitializeComponents();
    void SetupInventoryGrid();
    void LoadInventoryItems();
    void SynchronizeWithMainInventory();
};
#pragma endregion
/*-------------------------------------------------------------------------*/