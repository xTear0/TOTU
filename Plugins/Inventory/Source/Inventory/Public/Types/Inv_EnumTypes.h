// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "Inv_EnumTypes.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations										                   */
/*-------------------------------------------------------------------------*/
class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

UENUM(BlueprintType)
enum class EInv_ItemRarity : uint8
{
	Supreme,
	Mythical,
	Legendary,
	Epic,
	Rare,
	Uncommon,
	Common
};

UENUM(BlueprintType)
enum class EInv_ItemStar : uint8
{
	NoStars,
	OneStar,
	TwoStar,
	ThreeStar,
	FourStar,
	FiveStar,
	SixStar,
	SevenStar
};

UENUM(BlueprintType)
enum class EInv_Colors : uint8
{
	// Rarity Colors
	SupremeRed			UMETA(DisplayName = "Supreme Red"),
	MythicalGold		UMETA(DisplayName = "Mythical Gold"),
	LegendaryYellow		UMETA(DisplayName = "Legendary Yellow"),
	EpicPurple			UMETA(DisplayName = "Epic Purple"),
	RareBlue			UMETA(DisplayName = "Rare Blue"),
	UncommonGreen		UMETA(DisplayName = "Uncommon Green"),
	CommonGray			UMETA(DisplayName = "Common Gray"),

	// Attribute Colors
	StrengthYellow			UMETA(DisplayName = "Strength Bright Yellow"),
		SubAttributeYellow	UMETA(DisplayName = "Sub-Attribute Yellow"),
	IntelligenceBlue		UMETA(DisplayName = "Intelligence Bright Blue"),
		SubAttributeBlue	UMETA(DisplayName = "Sub-Attribute Blue"),
	FortitudeGreen			UMETA(DisplayName = "Fortitude Bright Green"),
		SubAttributeGreen	UMETA(DisplayName = "Sub-Attribute Green"),
	VitalityRed				UMETA(DisplayName = "Vitality Bright Red"),
		SubAttributeRed		UMETA(DisplayName = "Sub-Attribute Red"),
	
	// Damage Colors
	DamageTypeStatic		UMETA(DisplayName = "Damage Type Static"),
	DamageTypePhyiscal		UMETA(DisplayName = "DamageType - Physical Yellow"),
	DamageTypePoison		UMETA(DisplayName = "DamageType - Poison Green"),
	DamageTypeIce			UMETA(DisplayName = "DamageType - Ice Blue"),
	DamageTypeFire			UMETA(DisplayName = "DamageType - Fire Red"),
	DamageTypeLight			UMETA(DisplayName = "DamageType - Light White"),
	DamageTypeShadow		UMETA(DisplayName = "DamageType - Shadow Black"),
	
	// Colors Used By Text in UIs
	UIWhite				UMETA(DisplayName = "Basic UI White"),
	UITan				UMETA(DisplayName = "Basic UI Tan"),
	UIShadow			UMETA(DisplayName = "Basic UI Shadow")
};

UENUM(BlueprintType)
enum class EInv_ItemAttribute : uint8
{
	None 				UMETA(DisplayName = "None"),
	
	Strength  			UMETA(DisplayName = "Strength"),
	Intelligence 		UMETA(DisplayName = "Intelligence"),
	Fortitude 			UMETA(DisplayName = "Fortitude"),
	Vitality 			UMETA(DisplayName = "Vitality"),

	Dexterity  			UMETA(DisplayName = "Dexterity"),
	MaxMana				UMETA(DisplayName = "Maximum Mana"),
	ManaRegen			UMETA(DisplayName = "Mana Regeneration"),
	MaxHealth			UMETA(DisplayName = "Maximum Health"),
	HealthRegen			UMETA(DisplayName = "Health Regeneration"),
	CritChance			UMETA(DisplayName = "Crit Chance"),
	CritDamage			UMETA(DisplayName = "Crit Damage"),
	IgnoreDEF			UMETA(DisplayName = "Ignore Resistance"),			
	
	DamagePhysical		UMETA(DisplayName = "Physical Damage"),
	DamagePoison 		UMETA(DisplayName = "Poison Damage"),
	DamageIce 			UMETA(DisplayName = "Ice Damage"),
	DamageFire 			UMETA(DisplayName = "Fire Damage"),
	DamageLight 		UMETA(DisplayName = "Light Damage"),
	DamageShadow 		UMETA(DisplayName = "Shadow Damage"),

	ResistanceGlobal  		UMETA(DisplayName = "Global Resistance"),
	ResistancePhysical  	UMETA(DisplayName = "Physical Resistance"),
	ResistancePoison  		UMETA(DisplayName = "Poison Resistance"),
	ResistanceIce 			UMETA(DisplayName = "Ice Resistance"),
	ResistanceFire 			UMETA(DisplayName = "Fire Resistance"),
	ResistanceLight 		UMETA(DisplayName = "Light Resistance"),
	ResistanceShadow 		UMETA(DisplayName = "Shadow Resistance"),
	MAX UMETA(Hidden)
};

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability() {}
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}
	
	int32 Index{INDEX_NONE};
	int32 AmountToFill{0};
	bool bItemAtIndex{false};
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	TWeakObjectPtr<UInv_InventoryItem> Item;
	int32 TotalRoomToFill{0};
	int32 Remainder{0};
	bool bStackable{false};
	TArray<FInv_SlotAvailability> SlotAvailabilities;
	
};

UENUM(BlueprintType)
enum class EInv_TileQuadrant :  uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FIntPoint TileCoordinates{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& A, const  FInv_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates &&
		A.TileIndex == B.TileIndex &&
			A.TileQuadrant == B.TileQuadrant;
}

USTRUCT()
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()

	// True if the space queried has no items in it.
	bool bHasSpace{false};

	// Valid if there is a single item we can swap with.
	TWeakObjectPtr<UInv_InventoryItem> ValidItem = nullptr;

	// Upper left index of the valid item, if there is one.
	int32 UpperLeftIndex{INDEX_NONE};
};
/*-------------------------------------------------------------------------*/