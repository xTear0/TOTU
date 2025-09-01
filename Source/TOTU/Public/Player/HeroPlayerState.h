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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Connection Between Attributes and the Player                           */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerState.h_Class
UCLASS()
class TOTU_API AHeroPlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
public:
	AHeroPlayerState();

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
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetAbilityPoints() const { return AbilityPoints; }

	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	
	void AddToXP(int32 InXp);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InPoints);
	void AddToAbilityPoints(int32 InPoints);
	
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

	UFUNCTION() 
	void OnRep_Level(int32 OldLevel);

	UFUNCTION() 
	void OnRep_XP(int32 OldXP);

	UFUNCTION() 
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION() 
	void OnRep_AbilityPoints(int32 OldAbilityPoints);

};
#pragma endregion
/*-------------------------------------------------------------------------*/