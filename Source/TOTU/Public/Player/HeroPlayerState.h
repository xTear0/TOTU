// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "EquipmentManagement/Modifications/Inv_AttributeModificationHandler.h"
#include "Interaction/CombatInterface.h"
#include "HeroPlayerState.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Declarations													       */
/*-------------------------------------------------------------------------*/
class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /* StatValue */)

USTRUCT(BlueprintType)
struct FPlayerAttributes
{
	GENERATED_BODY()

	UPROPERTY()
	int32 PlayerStrength{0};

	UPROPERTY()
	int32 PlayerIntelligence{0};

	UPROPERTY()
	int32 PlayerFortitude{0};

	UPROPERTY()
	int32 PlayerVitality{0};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributesChanged, FPlayerAttributes /* Attributes */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerAttributeIDChanged, FGuid /* Attribute ID */)
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Connection Between Attributes and the Player                           */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerState.h_Class
UCLASS()
class TOTU_API AHeroPlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface, public IInv_AbilitySystemInterface
{
	GENERATED_BODY()
public:
	AHeroPlayerState();

	/*-------------------------------------------------------------------------*/
	// REQUIRED FOR INVENTORY COMPATIBILITY
	/*-------------------------------------------------------------------------*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponentForInventory() const override
	{
		return AbilitySystemComponent;
	}
    
	virtual UInv_AttributeModificationHandler* GetAttributeModificationHandlerForInventory() const override
	{
		return AttributeModificationHandler;
	}
	/*-------------------------------------------------------------------------*/
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintCallable, Category = "HeroPlayerState")
	UInv_AttributeModificationHandler* GetAttributeModificationHandler() const { return AttributeModificationHandler; }
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointChangedDelegate;
	FOnPlayerStatChanged OnAbilityPointChangedDelegate;
	FOnPlayerAttributesChanged OnAttributesChangedDelegate;
	FOnPlayerAttributeIDChanged OnAttributeIDChangedDelegate;
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetAbilityPoints() const { return AbilityPoints; }
	FORCEINLINE FPlayerAttributes GetPlayerAttributes() const { return PlayerAttributes; }
	FORCEINLINE FGuid GetPlayerAttributeID() const { return PlayerAttributeID; }
	
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	
	void AddToXP(int32 InXp);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);
	void AddToAbilityPoints(int32 InPoints);

	void CreateNewPlayerAttributeID();
	
	FORCEINLINE void SetStrength(int32 NewStrength) { PlayerAttributes.PlayerStrength = NewStrength; }
	FORCEINLINE void SetIntelligence(int32 NewIntelligence) { PlayerAttributes.PlayerIntelligence = NewIntelligence; }
	FORCEINLINE void SetFortitude(int32 NewFortitude) { PlayerAttributes.PlayerFortitude = NewFortitude; }
	FORCEINLINE void SetVitality(int32 NewVitality) { PlayerAttributes.PlayerVitality = NewVitality; }

	FORCEINLINE void AddToStrength(int32 InStrength) { PlayerAttributes.PlayerStrength += InStrength; }
	FORCEINLINE void AddToIntelligence(int32 InIntelligence) { PlayerAttributes.PlayerIntelligence += InIntelligence; }
	FORCEINLINE void AddToFortitude(int32 InFortitude) { PlayerAttributes.PlayerFortitude += InFortitude; }
	FORCEINLINE void AddToVitality(int32 InVitality) { PlayerAttributes.PlayerVitality += InVitality; }
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInv_AttributeModificationHandler> AttributeModificationHandler;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 XP = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AbilityPoints)
	int32 AbilityPoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PlayerAttributes)
	FPlayerAttributes PlayerAttributes;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PlayerAttributeID)
	FGuid PlayerAttributeID;
	
	UFUNCTION() 
	void OnRep_Level(int32 OldLevel);

	UFUNCTION() 
	void OnRep_XP(int32 OldXP);

	UFUNCTION() 
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION() 
	void OnRep_AbilityPoints(int32 OldAbilityPoints);

	UFUNCTION()
	void OnRep_PlayerAttributes(FPlayerAttributes OldPlayerAttributes);

	UFUNCTION()
	void OnRep_PlayerAttributeID(FGuid OldPlayerAttributeID);

};
#pragma endregion
/*-------------------------------------------------------------------------*/