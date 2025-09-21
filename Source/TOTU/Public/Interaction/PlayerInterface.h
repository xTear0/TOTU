// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Player/HeroPlayerState.h"
#include "PlayerInterface.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Player Interface Functionality                                         */
/*-------------------------------------------------------------------------*/
#pragma region PlayerInterface.h_Class
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class TOTU_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToAbilityPoints(int32 InAbilityPoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	FPlayerAttributes GetPlayerAttributes() const;

	UFUNCTION(BlueprintNativeEvent)
	FGuid GetPlayerAttributeID() const;

	UFUNCTION(BlueprintNativeEvent)
	void CreateNewPlayerAttributeID();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetStrength(int32 NewStrength);

	UFUNCTION(BlueprintNativeEvent)
	void SetIntelligence(int32 NewIntelligence);

	UFUNCTION(BlueprintNativeEvent)
	void SetFortitude(int32 NewFortitude);

	UFUNCTION(BlueprintNativeEvent)
	void SetVitality(int32 NewVitality);

	UFUNCTION(BlueprintNativeEvent)
	void AddToStrength(int32 InStrength);

	UFUNCTION(BlueprintNativeEvent)
	void AddToIntelligence(int32 InIntelligence);

	UFUNCTION(BlueprintNativeEvent)
	void AddToFortitude(int32 InFortitude);

	UFUNCTION(BlueprintNativeEvent)
	void AddToVitality(int32 InVitality);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
#pragma endregion
/*-------------------------------------------------------------------------*/