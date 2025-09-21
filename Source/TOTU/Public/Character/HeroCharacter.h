// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "Character/TOTUCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "HeroCharacter.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations				                                           */
/*-------------------------------------------------------------------------*/
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
struct FPlayerAttributes;
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Override and Init Functionality                                        */
/*-------------------------------------------------------------------------*/
#pragma region HeroCharacter.h_Class
UCLASS()
class TOTU_API AHeroCharacter : public ATOTUCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AHeroCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetAbilityPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToAbilityPoints_Implementation(int32 InAbilityPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetAbilityPoints_Implementation() const override;


	// Attribute Getters and Modifiers
	virtual FPlayerAttributes GetPlayerAttributes_Implementation() const override;
	virtual void CreateNewPlayerAttributeID_Implementation() override;
	virtual FGuid GetPlayerAttributeID_Implementation() const override;
	
	virtual void SetStrength_Implementation(int32 NewStrength) override;
	virtual void SetIntelligence_Implementation(int32 NewIntelligence) override;
	virtual void SetFortitude_Implementation(int32 NewFortitude) override;
	virtual void SetVitality_Implementation(int32 NewVitality) override;
	
	virtual void AddToStrength_Implementation(int32 InStrength) override;
	virtual void AddToIntelligence_Implementation(int32 InIntelligence) override;
	virtual void AddToFortitude_Implementation(int32 InFortitude) override;
	virtual void AddToVitality_Implementation(int32 InVitality) override;
	/* End Player Interface */


	
	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
protected:
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

private:
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
#pragma endregion
/*-------------------------------------------------------------------------*/