// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Widgets/Inventory/Spatial/Inv_SpatialStorage.h"
#include "Widgets/Inventory/Spatial/Inv_StorageGrid.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"
#include "InventoryManagement/Components/Inv_StorageComponent.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "InventoryManagement/Utils/Inv_InventoryStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inventory.h"
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
void UInv_SpatialStorage::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    // Check that ItemDescriptionClass is valid
    if (!ItemDescriptionClass)
    {
        UE_LOG(LogInventory, Error, TEXT("ItemDescriptionClass is null in SpatialStorage. Please set it in the Blueprint."));
    }
    
    InitializeComponents();
    SetupInventoryGrid();
    
    // Set up button callbacks
    if (IsValid(Button_PreviousPage))
    {
        Button_PreviousPage->OnClicked.AddDynamic(this, &ThisClass::OnPreviousPageClicked);
    }
    
    if (IsValid(Button_NextPage))
    {
        Button_NextPage->OnClicked.AddDynamic(this, &ThisClass::OnNextPageClicked);
    }
    
    if (IsValid(Button_AddPage))
    {
        Button_AddPage->OnClicked.AddDynamic(this, &ThisClass::OnPageAdded);
    }
    
    // Set up storage grid
    if (IsValid(Grid_Storage))
    {
        Grid_Storage->SetOwningCanvas(CanvasPanel);
        Grid_Storage->SetStorageComponent(StorageComponent);
        
        // IMPORTANT: Set the hover item reference from player inventory
        // This allows the storage grid to recognize hover items from player inventory
        if (IsValid(Grid_PlayerInventory))
        {
            Grid_Storage->SetHoverItemReference(Grid_PlayerInventory);
        }
    }
    
    // Set initial storage type
    SetStorageType(EInv_ItemCategory::Equippable);
    UpdatePageDisplay();
}

void UInv_SpatialStorage::InitializeComponents()
{
    APlayerController* PC = GetOwningPlayer();
    if (!IsValid(PC)) return;
    
    // Get or create storage component
    StorageComponent = PC->FindComponentByClass<UInv_StorageComponent>();
    if (!IsValid(StorageComponent))
    {
        StorageComponent = NewObject<UInv_StorageComponent>(PC);
        StorageComponent->RegisterComponent();
    }
    
    // Get inventory component
    InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(PC);
    
    if (IsValid(StorageComponent))
    {
        MaxPages = StorageComponent->GetMaxPages();
    }
}

void UInv_SpatialStorage::SetupInventoryGrid()
{
    if (!IsValid(Grid_PlayerInventory) || !IsValid(InventoryComponent)) return;
    
    // IMPORTANT: Use the same inventory component reference as the main inventory
    // This ensures both UIs share the same data
    if (IsValid(InventoryComponent.Get()))
    {
        Grid_PlayerInventory->SetInventoryComponent(InventoryComponent.Get());
    }
    Grid_PlayerInventory->SetAutoBindToInventory(false);
    Grid_PlayerInventory->SetOwningCanvas(CanvasPanel);
    
    // Clear any existing bindings first
    InventoryComponent->OnItemAdded.RemoveAll(Grid_PlayerInventory);
    InventoryComponent->OnStackChanged.RemoveAll(Grid_PlayerInventory);
    InventoryComponent->OnItemRemoved.RemoveAll(Grid_PlayerInventory);
    
    // Bind to inventory events
    InventoryComponent->OnItemAdded.AddDynamic(Grid_PlayerInventory, &UInv_InventoryGrid::AddItem);
    InventoryComponent->OnStackChanged.AddDynamic(Grid_PlayerInventory, &UInv_InventoryGrid::AddStacks);
    InventoryComponent->OnItemRemoved.AddDynamic(Grid_PlayerInventory, &UInv_InventoryGrid::OnItemRemoved);
    
    // Load existing items
    LoadInventoryItems();
}

void UInv_SpatialStorage::LoadInventoryItems()
{
    if (!IsValid(Grid_PlayerInventory) || !IsValid(InventoryComponent)) return;
    
    // Clear the grid first
    Grid_PlayerInventory->ClearGrid();
    
    // Load existing inventory items matching the current category
    const auto& InventoryList = InventoryComponent->GetInventoryList();
    for (UInv_InventoryItem* Item : InventoryList.GetAllItems())
    {
        if (IsValid(Item) && Item->GetItemManifest().GetItemCategory() == CurrentStorageType)
        {
            Grid_PlayerInventory->AddItem(Item);
        }
    }
}

void UInv_SpatialStorage::SynchronizeWithMainInventory()
{
    // This ensures the storage inventory grid stays in sync with the main inventory
    if (IsValid(Grid_PlayerInventory))
    {
        LoadInventoryItems();
    }
}

void UInv_SpatialStorage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // Handle cross-grid interaction
    HandleCrossGridInteraction();
    
    // Update item description position if visible
    if (IsValid(ItemDescription))
    {
        SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);
    }
}

void UInv_SpatialStorage::HandleCrossGridInteraction()
{
    // Update cross-grid hovering to show proper highlights
    UpdateCrossGridHovering();
}

void UInv_SpatialStorage::UpdateCrossGridHovering()
{
    FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
    
    // Get the grid that has a hover item
    UInv_InventoryGrid* ActiveGrid = GetActiveHoverGrid();
    if (!IsValid(ActiveGrid)) return;
    
    UInv_HoverItem* HoverItem = ActiveGrid->GetHoverItem();
    if (!IsValid(HoverItem)) return;
    
    // Determine which grid the mouse is over
    UInv_InventoryGrid* TargetGrid = GetTargetGrid(MousePos);
    if (!IsValid(TargetGrid)) return;
    
    // If hovering over a different grid, update its tile parameters
    if (ActiveGrid != TargetGrid)
    {
        // Set the hover item on the target grid temporarily for proper highlighting
        if (!TargetGrid->HasHoverItem())
        {
            // Get the grid dimensions from the hover item
            const FIntPoint Dimensions = HoverItem->GetGridDimensions();
            
            // Update the target grid's highlighting
            FVector2D GridPos = UInv_WidgetUtils::GetWidgetPosition(TargetGrid);
            TargetGrid->SetTemporaryHoverItem(HoverItem);
            TargetGrid->UpdateTileParameters(GridPos, MousePos);
        }
    }
    else
    {
        // Clear temporary hover items from other grids
        if (ActiveGrid == Grid_PlayerInventory && IsValid(Grid_Storage))
        {
            Grid_Storage->ClearTemporaryHoverItem();
        }
        else if (ActiveGrid == Grid_Storage && IsValid(Grid_PlayerInventory))
        {
            Grid_PlayerInventory->ClearTemporaryHoverItem();
        }
    }
}

FReply UInv_SpatialStorage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    if (TryTransferHoveredItem())
    {
        return FReply::Handled();
    }
    
    // Let grids handle their own drops
    if (IsValid(Grid_Storage))
        Grid_Storage->DropItem();
    
    if (IsValid(Grid_PlayerInventory))
        Grid_PlayerInventory->DropItem();
    
    return FReply::Handled();
}

bool UInv_SpatialStorage::TryTransferHoveredItem()
{
    FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
    
    UInv_InventoryGrid* SourceGrid = GetActiveHoverGrid();
    UInv_InventoryGrid* TargetGrid = GetTargetGrid(MousePos);
    
    if (!IsValid(SourceGrid) || !IsValid(TargetGrid) || SourceGrid == TargetGrid)
        return false;
    
    UInv_HoverItem* HoverItem = SourceGrid->GetHoverItem();
    if (!IsValid(HoverItem) || !IsValid(HoverItem->GetInventoryItem()))
        return false;
    
    UInv_InventoryItem* Item = HoverItem->GetInventoryItem();
    
    // Transfer from player inventory to storage
    if (SourceGrid == Grid_PlayerInventory && TargetGrid == Grid_Storage)
    {
        // Check if item matches storage type
        if (CurrentStorageType == EInv_ItemCategory::None || 
            Item->GetItemManifest().GetItemCategory() == CurrentStorageType)
        {
            // Add to storage
            StorageComponent->TryAddItemToStorage(Item, CurrentPageIndex);
            
            // Remove from inventory grid and actual inventory
            Grid_PlayerInventory->RemoveItemFromGrid(Item, HoverItem->GetPreviousGridIndex());
            InventoryComponent->Server_DropItem(Item, HoverItem->GetStackCount());
            
            // Clear hover item and temporary hover items
            SourceGrid->ClearHoverItem();
            TargetGrid->ClearTemporaryHoverItem();
            return true;
        }
    }
    // Transfer from storage to player inventory
    else if (SourceGrid == Grid_Storage && TargetGrid == Grid_PlayerInventory)
    {
        // Create a temporary item component for the transfer
        UInv_ItemComponent* TempItemComp = NewObject<UInv_ItemComponent>(this);
        TempItemComp->InitItemManifest(Item->GetItemManifest());
        
        if (Item->IsStackable())
        {
            FInv_ItemManifest TempManifest = Item->GetItemManifest();
            if (FInv_StackableFragment* StackableFragment = 
                TempManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
            {
                StackableFragment->SetStackCount(HoverItem->GetStackCount());
            }
            TempItemComp->InitItemManifest(TempManifest);
        }
        
        // Add to inventory
        InventoryComponent->TryAddItem(TempItemComp);
        
        // Remove from storage
        StorageComponent->TryRemoveItemFromStorage(Item);
        
        // Clear hover item and temporary hover items
        SourceGrid->ClearHoverItem();
        TargetGrid->ClearTemporaryHoverItem();
        return true;
    }
    
    return false;
}

UInv_InventoryGrid* UInv_SpatialStorage::GetActiveHoverGrid() const
{
    if (IsValid(Grid_PlayerInventory) && Grid_PlayerInventory->HasHoverItem())
        return Grid_PlayerInventory;
    if (IsValid(Grid_Storage) && Grid_Storage->HasHoverItem())
        return Grid_Storage;
    return nullptr;
}

UInv_InventoryGrid* UInv_SpatialStorage::GetTargetGrid(const FVector2D& MousePos) const
{
    // Check player inventory grid
    if (IsValid(Grid_PlayerInventory))
    {
        FVector2D GridPos = UInv_WidgetUtils::GetWidgetPosition(Grid_PlayerInventory);
        FVector2D GridSize = UInv_WidgetUtils::GetWidgetSize(Grid_PlayerInventory);
        if (UInv_WidgetUtils::IsWithinBounds(GridPos, GridSize, MousePos))
            return Grid_PlayerInventory;
    }
    
    // Check storage grid
    if (IsValid(Grid_Storage))
    {
        FVector2D GridPos = UInv_WidgetUtils::GetWidgetPosition(Grid_Storage);
        FVector2D GridSize = UInv_WidgetUtils::GetWidgetSize(Grid_Storage);
        if (UInv_WidgetUtils::IsWithinBounds(GridPos, GridSize, MousePos))
            return Grid_Storage;
    }
    
    return nullptr;
}

void UInv_SpatialStorage::SetStorageType(EInv_ItemCategory Category)
{
    CurrentStorageType = Category;
    
    if (IsValid(StorageComponent))
    {
        StorageComponent->SetStorageType(Category);
    }
    
    if (IsValid(Grid_Storage))
    {
        Grid_Storage->SetStorageType(Category);
    }
    
    // Update player inventory grid to show matching category
    if (IsValid(Grid_PlayerInventory))
    {
        Grid_PlayerInventory->SetItemCategory(Category);
        LoadInventoryItems();
    }
    
    // Update UI text
    if (IsValid(Text_StorageType))
    {
        FString TypeName;
        switch (Category)
        {
            case EInv_ItemCategory::Equippable:
                TypeName = "Equipment Storage";
                break;
            case EInv_ItemCategory::Consumable:
                TypeName = "Consumable Storage";
                break;
            case EInv_ItemCategory::Craftable:
                TypeName = "Craftable Storage";
                break;
            default:
                TypeName = "General Storage";
                break;
        }
        Text_StorageType->SetText(FText::FromString(TypeName));
    }
    
    RefreshStoragePage();
}

// Item Description Methods
void UInv_SpatialStorage::OnItemHovered(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    
    // Make sure we have a valid ItemDescriptionClass
    if (!ItemDescriptionClass)
    {
        UE_LOG(LogInventory, Warning, TEXT("ItemDescriptionClass is null, cannot create item description"));
        return;
    }
    
    const auto& Manifest = Item->GetItemManifest();
    EInv_ItemRarity Rarity = Manifest.GetItemRarity();
    UInv_ItemDescription* DescriptionWidget = GetItemDescription(Rarity, true);
    
    if (!IsValid(DescriptionWidget))
    {
        UE_LOG(LogInventory, Warning, TEXT("Failed to create ItemDescription widget"));
        return;
    }
    
    DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
    
    GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
    FTimerDelegate DescriptionTimerDelegate;
    DescriptionTimerDelegate.BindLambda([this, Item, Rarity]()
    {
        if (!IsValid(Item) || !IsValid(this)) return;
        
        UInv_ItemDescription* Widget = GetItemDescription(Rarity, true);
        if (IsValid(Widget))
        {
            Widget->SetVisibility(ESlateVisibility::HitTestInvisible);
            Item->GetItemManifest().AssimilateInventoryFragments(Widget);
        }
    });
    
    GetOwningPlayer()->GetWorldTimerManager().SetTimer(
        DescriptionTimer,
        DescriptionTimerDelegate,
        DescriptionTimerDelay,
        false
    );
}

void UInv_SpatialStorage::OnItemUnhovered()
{
    if (IsValid(ItemDescription))
    {
        ItemDescription->SetVisibility(ESlateVisibility::Collapsed);
    }
    GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UInv_SpatialStorage::HasHoverItem() const
{
    return (IsValid(Grid_PlayerInventory) && Grid_PlayerInventory->HasHoverItem()) ||
           (IsValid(Grid_Storage) && Grid_Storage->HasHoverItem());
}

UInv_HoverItem* UInv_SpatialStorage::GetHoverItem() const
{
    if (IsValid(Grid_PlayerInventory) && Grid_PlayerInventory->HasHoverItem())
        return Grid_PlayerInventory->GetHoverItem();
    if (IsValid(Grid_Storage) && Grid_Storage->HasHoverItem())
        return Grid_Storage->GetHoverItem();
    return nullptr;
}

UInv_ItemDescription* UInv_SpatialStorage::GetItemDescription(const EInv_ItemRarity& Rarity, bool UseRarity)
{
    if (!IsValid(ItemDescription))
    {
        if (!ItemDescriptionClass)
        {
            UE_LOG(LogInventory, Error, TEXT("Cannot create ItemDescription - ItemDescriptionClass is null"));
            return nullptr;
        }
        
        ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
        if (IsValid(ItemDescription) && IsValid(CanvasPanel))
        {
            CanvasPanel->AddChild(ItemDescription);
        }
    }
    
    if (IsValid(ItemDescription) && UseRarity) 
    {
        ItemDescription->SetRarity(Rarity);
    }
    
    return ItemDescription;
}

void UInv_SpatialStorage::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
    if (!IsValid(Description) || !IsValid(Canvas)) return;
    
    UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
    if (!IsValid(ItemDescriptionCPS)) return;
    
    const FVector2D ItemDescriptionSize = Description->GetBoxSize();
    ItemDescriptionCPS->SetSize(ItemDescriptionSize);
    
    FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
        UInv_WidgetUtils::GetWidgetSize(Canvas),
        ItemDescriptionSize,
        UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer())
    );
    
    ItemDescriptionCPS->SetPosition(ClampedPosition);
}

// Navigation methods remain the same...
void UInv_SpatialStorage::RefreshStoragePage()
{
    if (IsValid(Grid_Storage))
    {
        Grid_Storage->SetCurrentPage(CurrentPageIndex);
        Grid_Storage->RefreshPage();
    }
}

void UInv_SpatialStorage::SetCurrentPage(int32 PageIndex)
{
    CurrentPageIndex = FMath::Clamp(PageIndex, 0, MaxPages - 1);
    
    if (IsValid(StorageComponent))
    {
        StorageComponent->OnStoragePageChanged.Broadcast(CurrentPageIndex);
    }
    
    RefreshStoragePage();
    UpdatePageDisplay();
}

void UInv_SpatialStorage::OnPreviousPageClicked()
{
    if (CurrentPageIndex > 0)
    {
        SetCurrentPage(CurrentPageIndex - 1);
    }
}

void UInv_SpatialStorage::OnNextPageClicked()
{
    if (CurrentPageIndex < MaxPages - 1)
    {
        SetCurrentPage(CurrentPageIndex + 1);
    }
}

void UInv_SpatialStorage::OnPageAdded()
{
    if (MaxPages < 20 && IsValid(StorageComponent))
    {
        MaxPages++;
        StorageComponent->SetMaxPages(MaxPages);
        UpdatePageDisplay();
    }
}

void UInv_SpatialStorage::UpdatePageDisplay() const
{
    if (IsValid(Text_PageNumber))
    {
        FText PageText = FText::Format(
            FText::FromString("Page {0}/{1}"),
            FText::AsNumber(CurrentPageIndex + 1),
            FText::AsNumber(MaxPages)
        );
        Text_PageNumber->SetText(PageText);
    }
    
    if (IsValid(Button_PreviousPage))
    {
        Button_PreviousPage->SetIsEnabled(CurrentPageIndex > 0);
    }
    
    if (IsValid(Button_NextPage))
    {
        Button_NextPage->SetIsEnabled(CurrentPageIndex < MaxPages - 1);
    }
    
    if (IsValid(Button_AddPage))
    {
        Button_AddPage->SetIsEnabled(MaxPages < 20);
    }
}
/*-------------------------------------------------------------------------*/