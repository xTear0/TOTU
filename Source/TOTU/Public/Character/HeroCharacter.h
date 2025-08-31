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