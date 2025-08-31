// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "TOTUEffectActor.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Predeclarations                                                       */
/*-------------------------------------------------------------------------*/
class UGameplayEffect;
class UAbilitySystemComponent;

#pragma region ENUM_Declarations

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap, 
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Effect Actor Overlap Functionality                                     */
/*-------------------------------------------------------------------------*/
#pragma region TOTUEffectActor.h_Class
UCLASS()
class TOTU_API ATOTUEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATOTUEffectActor();
	
	//UPROPERTY(EditDefaultsOnly, Category = "Potion")
	//EPotionType PotionType;

protected:
	virtual void BeginPlay() override;

//	virtual void PostInitializeComponents() override;

//	FString GetPotionType() const;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectToEnemies = false;

/*-----------------------------------------------------------------*/
/*  Blueprint Instant Effects                                      */
/*-----------------------------------------------------------------*/
#pragma region Blueprint_Instant_Based_Gameplay_Effect
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

#pragma endregion
/*-----------------------------------------------------------------*/



/*-----------------------------------------------------------------*/
/*  Blueprint Duration Effects                                     */
/*-----------------------------------------------------------------*/
#pragma region Blueprint_Duration_Based_Gameplay_Effect

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

#pragma endregion
/*-----------------------------------------------------------------*/



/*-----------------------------------------------------------------*/
/*  Blueprint Infinite Effects                                     */
/*-----------------------------------------------------------------*/
#pragma region Blueprint_Infinite_Based_Gameplay_Effect

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "	Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

#pragma endregion
/*-----------------------------------------------------------------*/

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;

};
/*-------------------------------------------------------------------------*/
