// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/Inv_EnumTypes.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_StorageComponent.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class UInv_InventoryItem;
class UInv_ItemComponent;
class UInv_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStorageItemChange, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStoragePageChange, int32, NewPage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStorageMenuToggled, bool, bOpen);

// Storage entry for server-side storage
USTRUCT(BlueprintType)
struct FInv_StorageEntry : public FFastArraySerializerItem
{
    GENERATED_BODY()
    
    FInv_StorageEntry() {}
    
private:
    friend struct FInv_StorageFastArray;
    friend class UInv_StorageComponent;
    
    UPROPERTY()
    TObjectPtr<UInv_InventoryItem> Item = nullptr;
    
    UPROPERTY()
    int32 PageIndex = 0;
};

// Storage fast array
USTRUCT(BlueprintType)
struct FInv_StorageFastArray : public FFastArraySerializer
{
    GENERATED_BODY()
    
    FInv_StorageFastArray() : OwnerComponent(nullptr) {}
    FInv_StorageFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {}
    
    TArray<UInv_InventoryItem*> GetItemsOnPage(int32 PageIndex) const;
    UInv_InventoryItem* AddEntry(UInv_ItemComponent* ItemComponent, int32 PageIndex);
    UInv_InventoryItem* AddEntry(UInv_InventoryItem* Item, int32 PageIndex);
    void RemoveEntry(UInv_InventoryItem* Item);
    UInv_InventoryItem* FindFirstItemByTypeOnPage(const FGameplayTag& ItemType, int32 PageIndex);
    
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
    {
        return FastArrayDeltaSerialize<FInv_StorageEntry, FInv_StorageFastArray>(Entries, DeltaParams, *this);
    }
    
    void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
    void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
    
private:
    friend class UInv_StorageComponent;
    
    UPROPERTY()
    TArray<FInv_StorageEntry> Entries;
    
    UPROPERTY(NotReplicated)
    TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInv_StorageFastArray> : TStructOpsTypeTraitsBase2<FInv_StorageFastArray>
{
    enum { WithNetDeltaSerializer = true };
};
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Class Functionality                                                   */
/*-------------------------------------------------------------------------*/
#pragma region Inv_StorageComponent.h_Class
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_StorageComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UInv_StorageComponent();
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void TryAddItemToStorage(UInv_InventoryItem* Item, int32 PageIndex);
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void TryRemoveItemFromStorage(UInv_InventoryItem* Item);
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetStorageType(EInv_ItemCategory Category) { StorageItemCategory = Category; }
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    EInv_ItemCategory GetStorageType() const { return StorageItemCategory; }
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    int32 GetMaxPages() const { return MaxPages; }
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetMaxPages(int32 Pages) { MaxPages = FMath::Clamp(Pages, 1, 20); }
    
    UFUNCTION(BlueprintCallable, Category = "Storage")
    TArray<UInv_InventoryItem*> GetItemsOnPage(int32 PageIndex) const;
    
    // RPCs
    UFUNCTION(Server, Reliable)
    void Server_AddItemToStorage(UInv_InventoryItem* Item, int32 PageIndex, int32 StackCount);
    
    UFUNCTION(Server, Reliable)
    void Server_RemoveItemFromStorage(UInv_InventoryItem* Item, int32 StackCount);
    
    UFUNCTION(Server, Reliable)
    void Server_TransferToInventory(UInv_InventoryItem* StorageItem, int32 StackCount);
    
    UFUNCTION(Server, Reliable)
    void Server_TransferToStorage(UInv_InventoryItem* InventoryItem, int32 PageIndex, int32 StackCount);
    
    // Delegates
    FStorageItemChange OnStorageItemAdded;
    FStorageItemChange OnStorageItemRemoved;
    FStoragePageChange OnStoragePageChanged;
    FStorageMenuToggled OnStorageMenuToggled;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool bIsMenuOpen() const { return bStorageMenuOpen; }

    void ToggleStorageMenu();

    
protected:
    virtual void BeginPlay() override;
    
private:
    
    TWeakObjectPtr<APlayerController> OwningController;
    
    bool bStorageMenuOpen = false;
    void OpenStorageMenu();
    void CloseStorageMenu();

    void ConstructStorageMenu();

    UPROPERTY()
    TObjectPtr<UInv_InventoryBase> StorageMenu;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UInv_InventoryBase> StorageMenuClass;
    
    UPROPERTY(Replicated)
    FInv_StorageFastArray StorageList;
    
    UPROPERTY(EditAnywhere, Category = "Storage")
    EInv_ItemCategory StorageItemCategory = EInv_ItemCategory::None;
    
    UPROPERTY(EditAnywhere, Category = "Storage")
    int32 MaxPages = 3;
    
    UPROPERTY(Replicated)
    int32 CurrentPage = 0;
    
    void AddRepSubObj(UObject* SubObj);
};
#pragma endregion
/*-------------------------------------------------------------------------*/