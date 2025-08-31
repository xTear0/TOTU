// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Player/HeroPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Input/TOTUInputComponent.h"
#include "TOTUGameplayTags.h"
#include "Character/TOTUEnemy.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "Character/HeroCharacter.h"
#include "GameFramework/Character.h"
#include "Interaction/Inv_Highlightable.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Widgets/HUD/Inv_HUDWidget.h"
#include "Interaction/Inv_HighlightableStaticMesh.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerController.cpp_Constructor
AHeroPlayerController::AHeroPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	EnemyTraceLength = 20000.0;
	ItemTraceLength = 2000.0;
	ItemTraceChannel = ECollisionChannel::ECC_GameTraceChannel1;
	EnemyTraceChannel = ECollisionChannel::ECC_GameTraceChannel3;
}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region HeroPlayerController.cpp_Functions
void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(HeroContext);

	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(HeroContext, 0);
	}
	// Settings for Inventory System
	CreateHUDWidget();

	
	// Settings for Player Controller
	DefaultMouseCursor = EMouseCursor::Default;
	bShowMouseCursor = false;

	SetInputMode(GameOnlyInputMode);
}

void AHeroPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TraceForEnemy();
	TraceForItem();
}

void AHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UTOTUInputComponent* TOTUInputComponent = CastChecked<UTOTUInputComponent>(InputComponent);

	// Player Actions
	TOTUInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AHeroPlayerController::ShiftPressed);
	TOTUInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AHeroPlayerController::ShiftReleased);
	TOTUInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeroPlayerController::Move);
	TOTUInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeroPlayerController::Look);
	
	// Inventory Actions
	TOTUInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AHeroPlayerController::PrimaryInteract);
	TOTUInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AHeroPlayerController::ToggleInventory);
	
	TOTUInputComponent->BindAbilityActions(InputConfig, this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld);
}

void AHeroPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsDodgedHit, bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->SetInitialLayerZOrder(10);
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bIsDodgedHit, bIsCriticalHit);
	}
}

void AHeroPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// find out which way is forward
		const FRotator Rotation = ControlledPawn->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AHeroPlayerController::Look(const FInputActionValue& LookValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = LookValue.Get<FVector2D>();

	if (AHeroCharacter* Hero = Cast<AHeroCharacter>(GetPawn()) )
	{
		Hero->AddControllerYawInput(LookAxisVector.X);
		Hero->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHeroPlayerController::TraceForEnemy()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter,  TraceStart, Forward)) return;
	const FVector TraceEnd = TraceStart + Forward * EnemyTraceLength;
	FHitResult EnemyHitResult;
	GetWorld()->LineTraceSingleByChannel(EnemyHitResult, TraceStart, TraceEnd, EnemyTraceChannel);
	
	Enemy_LastActor = Enemy_ThisActor;
	Enemy_ThisActor = EnemyHitResult.GetActor();

	if (Enemy_ThisActor == Enemy_LastActor) return;

	if (Enemy_LastActor)
	{
		Enemy_LastActor->UnHighlightActor();
	}

	if (Enemy_ThisActor)
	{
		Enemy_ThisActor->HighlightActor();
		TargetImpactPoint = EnemyHitResult.ImpactPoint;
	}
	else
	{
		TargetImpactPoint = FVector::ZeroVector;
	}
}


void AHeroPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FTOTUGameplayTags::Get().InputTag_LMB))
	{
		bTargetting = Enemy_ThisActor ? true : false;
	}
}

void AHeroPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FTOTUGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
	if (!bTargetting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		bTargetting = false;
	}		
}

void AHeroPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FTOTUGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargetting || bShiftKeyDown)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
}

UTOTUAbilitySystemComponent* AHeroPlayerController::GetASC()
{
	if (TOTUAbilitySystemComponent == nullptr) 
	{
		TOTUAbilitySystemComponent = Cast<UTOTUAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TOTUAbilitySystemComponent;
}

void AHeroPlayerController::ToggleInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Toggle inventory"));

	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();
	
	if (InventoryComponent->bIsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

FVector AHeroPlayerController::GetTargetImpactPoint() const
{
	return TargetImpactPoint;
}

void AHeroPlayerController::PrimaryInteract()
{
	if (!Inv_ThisActor.IsValid()) return;

	UInv_ItemComponent* ItemComp = Inv_ThisActor->FindComponentByClass<UInv_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;
	
	InventoryComponent->TryAddItem(ItemComp);
}

void AHeroPlayerController::CreateHUDWidget()
{
	if (!IsLocalPlayerController()) return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}



/*-------------------------------------------------------------------------*/
/* Inventory System Line Trace for Item by Blueprint Set Channel		   */
/*-------------------------------------------------------------------------*/
void AHeroPlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter,  TraceStart, Forward)) return;
	const FVector TraceEnd = TraceStart + Forward * ItemTraceLength;
	FHitResult ItemHitResult;
	GetWorld()->LineTraceSingleByChannel(ItemHitResult, TraceStart, TraceEnd, ItemTraceChannel);

	
	Inv_LastActor = Inv_ThisActor;
	Inv_ThisActor = ItemHitResult.GetActor();

	if (!Inv_ThisActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}
	
	if (Inv_ThisActor == Inv_LastActor) return;
	if (Inv_ThisActor.IsValid())
	{
		if	(UActorComponent* Highlightable = Inv_ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_Highlight(Highlightable);
		}
		UInv_ItemComponent* ItemComponent = Inv_ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
	}
	
	if (Inv_LastActor.IsValid())
	{
		if	(UActorComponent* Highlightable = Inv_LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}

	// If we were looking at a valid object but now we aren't, we should also unhighlight.
	if (Inv_LastActor.IsValid() && !Inv_ThisActor.IsValid())
	{
		if	(UActorComponent* Highlightable = Inv_LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			IInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
	}
}
#pragma endregion
/*-------------------------------------------------------------------------*/