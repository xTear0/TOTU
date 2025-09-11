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
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
void UInv_SpatialStorage::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    InitializeComponents();
    
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
    
    // Set up grids
    if (IsValid(Grid_Storage))
    {
        Grid_Storage->SetOwningCanvas(CanvasPanel);
        Grid_Storage->SetStorageComponent(StorageComponent);
    }
    
    if (IsValid(Grid_PlayerInventory))
    {
        Grid_PlayerInventory->SetOwningCanvas(CanvasPanel);
    }

    // Connect Grid_PlayerInventory to the actual inventory component
    if (IsValid(Grid_PlayerInventory) && IsValid(InventoryComponent))
    {
        Grid_PlayerInventory->SetOwningCanvas(CanvasPanel);
        
        // Bind to inventory component events
        InventoryComponent->OnItemAdded.AddDynamic(Grid_PlayerInventory, &UInv_InventoryGrid::AddItem);
        InventoryComponent->OnStackChanged.AddDynamic(Grid_PlayerInventory, &UInv_InventoryGrid::AddStacks);
        
        // Load existing inventory items
        for (UInv_InventoryItem* Item : InventoryComponent->GetInventoryList().GetAllItems())
        {
            if (IsValid(Item) && Item->GetItemManifest().GetItemCategory() == Grid_PlayerInventory->GetItemCategory())
            {
                Grid_PlayerInventory->AddItem(Item);
            }
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

void UInv_SpatialStorage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    if (Grid_Storage->HasHoverItem() || Grid_PlayerInventory->HasHoverItem())
    {
        HandleCrossGridTransfer();
    }
    
    if (IsValid(ItemDescription))
    {
        SetItemDescriptionSizeAndPosition(ItemDescription, CanvasPanel);
    }
}

FReply UInv_SpatialStorage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& MouseEvent)
{
    FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
    
    // Check if we're dropping on storage grid
    if (Grid_PlayerInventory->HasHoverItem())
    {
        FVector2D StoragePos = UInv_WidgetUtils::GetWidgetPosition(Grid_Storage);
        FVector2D StorageSize = UInv_WidgetUtils::GetWidgetSize(Grid_Storage);
        
        if (UInv_WidgetUtils::IsWithinBounds(StoragePos, StorageSize, MousePos))
        {
            // Transfer item to storage
            UInv_InventoryItem* HoverItem = Grid_PlayerInventory->GetHoverItem()->GetInventoryItem();
            if (IsValid(HoverItem))
            {
                TransferItemToStorage(HoverItem);
                Grid_PlayerInventory->ClearHoverItem();
                return FReply::Handled();
            }
        }
    }
    // Similar logic for storage to inventory transfer
    
    return Super::NativeOnMouseButtonDown(InGeometry, MouseEvent);
}

void UInv_SpatialStorage::HandleCrossGridTransfer()
{
    FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
    
    // Check which grid the mouse is over
    if (Grid_Storage->HasHoverItem())
    {
        // If hovering over player inventory grid
        FVector2D GridPos = UInv_WidgetUtils::GetWidgetPosition(Grid_PlayerInventory);
        FVector2D GridSize = UInv_WidgetUtils::GetWidgetSize(Grid_PlayerInventory);
        
        if (UInv_WidgetUtils::IsWithinBounds(GridPos, GridSize, MousePos))
        {
            // Allow Grid_PlayerInventory to handle the hover item
            Grid_PlayerInventory->UpdateTileParameters(GridPos, MousePos);
        }
    }
    else if (Grid_PlayerInventory->HasHoverItem())
    {
        // Similar logic for storage grid
        FVector2D GridPos = UInv_WidgetUtils::GetWidgetPosition(Grid_Storage);
        FVector2D GridSize = UInv_WidgetUtils::GetWidgetSize(Grid_Storage);
        
        if (UInv_WidgetUtils::IsWithinBounds(GridPos, GridSize, MousePos))
        {
            Grid_Storage->UpdateTileParameters(GridPos, MousePos);
        }
    }
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

void UInv_SpatialStorage::UpdatePageDisplay()
{
    // Update page number text
    if (IsValid(Text_PageNumber))
    {
        FText PageText = FText::Format(
            FText::FromString("Page {0}/{1}"),
            FText::AsNumber(CurrentPageIndex + 1),
            FText::AsNumber(MaxPages)
        );
        Text_PageNumber->SetText(PageText);
    }
    
    // Update button states
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

void UInv_SpatialStorage::OnItemHovered(UInv_InventoryItem* Item)
{
    if (!IsValid(Item)) return;
    
    const auto& Manifest = Item->GetItemManifest();
    EInv_ItemRarity Rarity = Manifest.GetItemRarity();
    UInv_ItemDescription* DescriptionWidget = GetItemDescription();
    DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);
    
    GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
    FTimerDelegate DescriptionTimerDelegate;
    DescriptionTimerDelegate.BindLambda([this, &Manifest, Rarity, DescriptionWidget]()
    {
        GetItemDescription(Rarity, true)->SetVisibility(ESlateVisibility::HitTestInvisible);
        Manifest.AssimilateInventoryFragments(DescriptionWidget);
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
    GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
    GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

void UInv_SpatialStorage::TransferItemToStorage(UInv_InventoryItem* Item)
{
    if (!IsValid(Item) || !IsValid(StorageComponent)) return;
    
    // Check if item matches storage type
    if (CurrentStorageType != EInv_ItemCategory::None && 
        Item->GetItemManifest().GetItemCategory() != CurrentStorageType)
    {
        return;
    }
    
    StorageComponent->Server_TransferToStorage(Item, CurrentPageIndex, Item->GetTotalStackCount());
}

void UInv_SpatialStorage::TransferItemToInventory(UInv_InventoryItem* Item)
{
    if (!IsValid(Item) || !IsValid(StorageComponent)) return;
    
    StorageComponent->Server_TransferToInventory(Item, Item->GetTotalStackCount());
}

void UInv_SpatialStorage::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const
{
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

UInv_ItemDescription* UInv_SpatialStorage::GetItemDescription(const EInv_ItemRarity& Rarity, bool UseRarity)
{
    if (!IsValid(ItemDescription))
    {
        ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
        CanvasPanel->AddChild(ItemDescription);
    }
    
    if (UseRarity)
    {
        ItemDescription->SetRarity(Rarity);
    }
    
    return ItemDescription;
}
/*-------------------------------------------------------------------------*/