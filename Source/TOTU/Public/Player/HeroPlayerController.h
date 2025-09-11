// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "InventoryManagement/Components/Inv_InventoryComponent.h"
#include "HeroPlayerController.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations                                                          */
/*-------------------------------------------------------------------------*/
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UTOTUInputConfig;
class UTOTUAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class UInv_HUDWidget;
class UInv_StorageComponent;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Player Movement, Mouse Checks, and Inventory PC System                 */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerController.h_Class
UCLASS()
class TOTU_API AHeroPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
	AHeroPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsDodgedHit, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void TempToggleStorage();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	FVector GetTargetImpactPoint() const;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HeroContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Look(const FInputActionValue& LookValue);
	void Move(const FInputActionValue& InputActionValue);


	/*--------------------------------------------------------------*/
	/*  Enemy Player Controller Systems					            */
	/*--------------------------------------------------------------*/
	void TraceForEnemy();
	
	// Stores information of the current target enemy.
	FVector TargetImpactPoint;
	
	TScriptInterface<IEnemyInterface> Enemy_LastActor;
	TScriptInterface<IEnemyInterface> Enemy_ThisActor;
	FHitResult EnemyCursorHit;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	double EnemyTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TEnumAsByte<ECollisionChannel> EnemyTraceChannel;


	/*--------------------------------------------------------------*/
	/*  Ability Player Controller Systems				            */
	/*--------------------------------------------------------------*/
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTOTUInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UTOTUAbilitySystemComponent> TOTUAbilitySystemComponent;

	UTOTUAbilitySystemComponent* GetASC();
	
	bool bTargetting = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	/*--------------------------------------------------------------*/
	/*  Inventory Player Controller Systems				            */
	/*--------------------------------------------------------------*/
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<UInv_StorageComponent> StorageComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> TempToggleStorageAction; // TODO: Replace this with an input when hovering on a storage container
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double ItemTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;
	
	TWeakObjectPtr<AActor> Inv_ThisActor;
	TWeakObjectPtr<AActor> Inv_LastActor;
	FHitResult Inv_CursorHit;

	FInputModeGameOnly GameOnlyInputMode;
	FInputModeUIOnly UIOnlyInputMode;
	FInputModeGameAndUI GameAndUIInputMode;
};
#pragma endregion
/*-------------------------------------------------------------------------*/