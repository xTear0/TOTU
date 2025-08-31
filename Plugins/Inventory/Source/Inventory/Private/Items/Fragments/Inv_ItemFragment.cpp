// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Items/Fragments/Inv_ItemFragment.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EquipmentManagement/EquipActor/Inv_EquipActor.h"
#include "EquipmentManagement/Modifications/Inv_EquippedItemPayload.h"
#include "GameFramework/PlayerState.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "TOTU/Public/Player/HeroPlayerState.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabeledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"
#include "Widgets/Composite/Inv_Leaf_ValuedLabel.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region Inv_ItemFragment.cpp_Functions
void FInv_LabeledNumberFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
}

void FInv_NumberedLabelFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
}

void FInv_TextFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
}

FText FInv_ItemFragment::QueryManifestForItemName() const
{
	if (OwningManifest != nullptr)
	{
		return OwningManifest->GetItemName(); 
	}
	return FText::GetEmpty();
}

FText FInv_ItemFragment::QueryManifestForItemDescription() const
{
	if (OwningManifest != nullptr)
	{
		return OwningManifest->GetItemDescription(); 
	}
	return FText::GetEmpty();
}

EInv_ItemRarity FInv_ItemFragment::QueryItemManifestForRarity() const
{
	if (OwningManifest != nullptr)
	{
		return OwningManifest->GetItemRarity(); 
	}
	return EInv_ItemRarity::Common;
}

int32 FInv_ItemFragment::QueryItemManifestForAttributeValue(FGameplayTag AttributeTag) const
{
	if (OwningManifest != nullptr)
	{
		return OwningManifest->GetItemAttributeValue(AttributeTag);
	}
	return 0;
}

int32 FInv_ItemFragment::QueryItemManifestForSellValue() const
{
	if (OwningManifest != nullptr)
	{
		return OwningManifest->GetItemSellValue();
	}
	return 0;
}


void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;
	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);
}



void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Text* LeafText = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(LeafText)) return;

	if (bPrefixWithRarity)
	{
		// Adds the Rarity of the Item to the Text with a space, i.e., Legendary Weapon. (Legendary )
		const FText RarityPrefix = UInv_WidgetUtils::GetPrefixFromRarityEnum(QueryItemManifestForRarity());
		LeafText->SetText(FText::Format(FText::FromString(TEXT("{0} {1}")), RarityPrefix, FragmentText));
	}
	if (!bPrefixWithRarity) LeafText->SetText(FragmentText);
	if (bUseItemNameForText) LeafText->SetText(QueryManifestForItemName());
	if (bUseItemDescriptionForText) LeafText->SetText(QueryManifestForItemDescription());
	if (bOverrideRarityTextColor) LeafText->SetColorAndOpacity(UInv_WidgetUtils::GetColorFromColorEnum(TextColor));
	if (!bOverrideRarityTextColor) LeafText->SetColorAndOpacity(UInv_WidgetUtils::GetColorFromRarityEnum(QueryItemManifestForRarity()));
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;
	
	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);
	LabeledValue->SetText_LabelColorAs(UInv_WidgetUtils::GetColorFromColorEnum(LabelColor));
	
	FNumberFormattingOptions Options;

	// Creae a sign prefix depending on if the value is Positive or Negative:
	
	FText NumberText = FText::AsNumber(Value, &Options);

	FText Prefix = Value < 0
	? FText::FromString(TEXT("-"))
	: FText::FromString(TEXT("+"));

	FText DisplayValueText = FText::Format(NSLOCTEXT("MyNamespace", "PlusMinusFmt", "{0}{1}"),Prefix,NumberText);
	
	FText DisplaySellText = FText::AsNumber(QueryItemManifestForSellValue(), &Options);
	
	if (!bUseSellValueForValue) LabeledValue->SetText_Value(DisplayValueText, bCollapseValue);
	if (bUseSellValueForValue) LabeledValue->SetText_Value(DisplaySellText, bCollapseValue);
	LabeledValue->SetText_ValueColorAs(UInv_WidgetUtils::GetColorFromColorEnum(ValueColor));
}

void FInv_NumberedLabelFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;
	
	UInv_Leaf_ValuedLabel* ValuedLabel = Cast<UInv_Leaf_ValuedLabel>(Composite);
	if (!IsValid(ValuedLabel)) return;
	
	FText AttributeText = UInv_WidgetUtils::GetAttributeTextLabel(AttributeTag);
	int32 AttributeValue = QueryItemManifestForAttributeValue(AttributeTag);
	FText SignedAttributeValue = UInv_WidgetUtils::GetSignedAttributeValueFromInt(AttributeValue);
	FLinearColor AttributeColor = UInv_WidgetUtils::GetColorFromAttributeTag(AttributeTag);
	
	ValuedLabel->SetAttributeValueAndTextWithColor(SignedAttributeValue, AttributeText, AttributeColor);
}

float FInv_LabeledNumberFragment::GetValue() const
{
	return Value;
}

void FInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : ConsumeModifers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : ConsumeModifers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FInv_PotionFragment::OnConsume(APlayerController* PC)
{
	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid GameplayEffectClass"));
		return;
	}

	UAbilitySystemComponent* TargetASC = PC->GetPlayerState<APlayerState>()->GetComponentByClass<UAbilitySystemComponent>();
	if (!TargetASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent not found on PC"));
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, GetEffectLevel(), EffectContextHandle);

	if (!EffectSpecHandle.IsValid() || !EffectSpecHandle.Data.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid EffectSpecHandle"));
		return;
	}

	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
		TEXT("Health Potion Consumed! +%f HP"), GetValue()));
}

/*-------------------------------------------------------------------------*/
/*   Unequip and Equip Functions for Equipment							   */
/*-------------------------------------------------------------------------*/

// For each subfragment, call OnEquip and Unequip
void FInv_EquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	// Create Equipped Item Payload
	FInv_EquippedItemPayload Payload = FInv_EquippedItemPayload::Create(
	FName(*OwningManifest->GetItemName().ToString()),
	UInv_WidgetUtils::GetColorFromRarityEnum(OwningManifest->GetItemRarity()),
	OwningManifest->GetItemAttributes(),
	OwningManifest->GetItemAbilities()
	);
	
	// Keep track of Payload for Unequip Removal.
	EquippedPayloadID = Payload.PayloadID;

	AHeroPlayerState* HeroPlayerState = PC->GetPlayerState<AHeroPlayerState>();
	UInv_AttributeModificationHandler* AttributeHandler = HeroPlayerState->GetAttributeModificationHandler();
	if (IsValid(AttributeHandler)) AttributeHandler->AddPayload(Payload.PayloadID, Payload);
}

void FInv_EquipmentFragment::OnUnequip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;

	AHeroPlayerState* HeroPlayerState = PC->GetPlayerState<AHeroPlayerState>();
	UInv_AttributeModificationHandler* AttributeHandler = HeroPlayerState->GetAttributeModificationHandler();
	if (IsValid(AttributeHandler)) AttributeHandler->RemovePayload(EquippedPayloadID);
		
	EquippedPayloadID.Invalidate();
}
/*-------------------------------------------------------------------------*/

void FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_EquipmentFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest(); // Set manifest/component on this
	
	// Add to the Array of EquipAttributeModifiers:
	if (!OwningManifest) return;

	// Loop through AttributePairs in the Manifest
	for (const FInv_ItemAttributePair& Pair : OwningManifest->GetItemAttributes())
	{
		// Add a new instance to the internal array
		TInstancedStruct<FInv_EquipModifier>& NewMod = EquipModifiers.Add_GetRef(TInstancedStruct<FInv_EquipModifier>());
		NewMod.InitializeAs<FInv_EquipModifier>();

		// Fill it in
			FInv_EquipModifier& Mod = NewMod.GetMutable();
			Mod.SetAttribute(Pair.AttributeTag);
			Mod.SetFragmentTag(Pair.FragmentTag);
			Mod.SetOwningManifest(GetOwningManifest());
			Mod.Manifest();
	}
}

AInv_EquipActor* FInv_EquipmentFragment::SpawnAttachedActor(USkeletalMeshComponent* AttachMesh, bool bProxy) const
{
	if (!IsValid(EquipActorClass) || !IsValid(AttachMesh))
		return nullptr;

	AInv_EquipActor* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<AInv_EquipActor>(EquipActorClass);
	if (!IsValid(SpawnedActor))
		return nullptr;

	// Default attach
	SpawnedActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// If this is a proxy, skip skeletal binding logic
	if (bProxy)
	{
		if (USceneComponent* BindMeshProxyComponent = Cast<USceneComponent>(
			AttachMesh->GetOwner()->FindComponentByTag(USkeletalMeshComponent::StaticClass(), FName("BindMesh"))))
		{
			SpawnedActor->AttachToComponent(BindMeshProxyComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketAttachPoint);
		}
		if (!bIsWeapon)
		{
			USkeletalMeshComponent* BindMeshProxySKM = Cast<USkeletalMeshComponent>(AttachMesh->GetOwner()->FindComponentByTag(USkeletalMeshComponent::StaticClass(), FName("BindMesh")));
			USkeletalMeshComponent* AddedActorSKM = Cast<USkeletalMeshComponent>(SpawnedActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
			AddedActorSKM->SetLeaderPoseComponent(BindMeshProxySKM);
		}
		return SpawnedActor;
	}

	// Try to bind the skeletal mesh to the character's master pose
	if (USkeletalMeshComponent* AddedActorSKM = Cast<USkeletalMeshComponent>(
		SpawnedActor->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
	{
		USkeletalMeshComponent* BindMeshSKM = Cast<USkeletalMeshComponent>(
			AttachMesh->GetOwner()->FindComponentByTag(USkeletalMeshComponent::StaticClass(), FName("BindMesh")));
		USkeletalMeshComponent* BindWeaponSKM = Cast<USkeletalMeshComponent>(
			AttachMesh->GetOwner()->FindComponentByTag(USkeletalMeshComponent::StaticClass(), FName("BindWeapon")));

		if (BindMeshSKM)
		{
			AddedActorSKM->SetLeaderPoseComponent(BindMeshSKM);
		}

		// Handle weapon override
		if (bIsWeapon && BindWeaponSKM)
		{
			USkeletalMesh* WeaponMesh = AddedActorSKM->GetSkeletalMeshAsset();
			BindWeaponSKM->SetSkeletalMesh(WeaponMesh);
			AddedActorSKM->SetVisibility(false);
			BindWeaponSKM->SetVisibility(true);
		}
	}

	return SpawnedActor;
}


void FInv_EquipmentFragment::DestroyAttachedActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}

void FInv_EquipmentFragment::SetEquippedActor(AInv_EquipActor* EquipActor)
{
	EquippedActor = EquipActor;
}
#pragma endregion
/*-------------------------------------------------------------------------*/


