// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#include "Character/TOTUEnemy.h"
#include "TOTU/TOTU.h"
#include "AI/TOTUAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystem/TOTUAbilitySystemComponent.h"
#include "AbilitySystem/TOTUAbilitySystemLibrary.h"
#include "AbilitySystem/TOTUAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/TOTUUserWidget.h"
#include "TOTUGameplayTags.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Constructor                                                           */
/*-------------------------------------------------------------------------*/
#pragma region TOTUEnemy.cpp_Constructor
ATOTUEnemy::ATOTUEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTOTUAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UTOTUAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}
void ATOTUEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	if (!HasAuthority()) return;
	TOTUAIController = Cast<ATOTUAIController>(NewController);
	TOTUAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	TOTUAIController->RunBehaviorTree(BehaviorTree);
	TOTUAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	TOTUAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass == ECharacterClass::Archer || CharacterClass == ECharacterClass::Arcanist);

}
#pragma endregion
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*   Functions                                                             */
/*-------------------------------------------------------------------------*/
#pragma region TOTUEnemy.cpp_Functions
void ATOTUEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATOTUEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 ATOTUEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void ATOTUEnemy::Die()
{
	SetLifeSpan(Lifespan);
	if (TOTUAIController) TOTUAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die();
}

void ATOTUEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ATOTUEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ATOTUEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();

	if (HasAuthority()) 
	{
		UTOTUAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	 
	if (UTOTUUserWidget* TOTUUserWidget = Cast<UTOTUUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		TOTUUserWidget->SetWidgetController(this);
	}


	if (UTOTUAttributeSet* TOTUAS = Cast<UTOTUAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TOTUAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) 
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TOTUAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FTOTUGameplayTags::Get().Effects_HitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
				this,
				&ATOTUEnemy::HitReactTagChanged
			);

		OnHealthChanged.Broadcast(TOTUAS->GetHealth());
		OnMaxHealthChanged.Broadcast(TOTUAS->GetMaxHealth());
	}
}

void ATOTUEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if	(TOTUAIController && TOTUAIController->GetBlackboardComponent())
	{
		TOTUAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}

}

void ATOTUEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTOTUAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority()) 
	{
		InitializeDefaultAttributes();
	}

}
void ATOTUEnemy::InitializeDefaultAttributes() const
{
	UTOTUAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
#pragma endregion
/*-------------------------------------------------------------------------*/