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
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }

	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	
	void AddToXP(int32 InXp);
	void AddToLevel(int32 InLevel);
	
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
	int32 XP = 0;

	UFUNCTION() 
	void OnRep_Level(int32 OldLevel);

	UFUNCTION() 
	void OnRep_XP(int32 OldXP);

};
#pragma endregion
/*-------------------------------------------------------------------------*/