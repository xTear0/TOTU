// Copyright xTear Studios
/*-------------------------------------------------------------------------*/
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "TOTUAIController.generated.h"
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*  Declarations                                                           */
/*-------------------------------------------------------------------------*/
class UBlackboardComponent;
class UBehaviorTreeComponent;
/*-------------------------------------------------------------------------*/




/*-------------------------------------------------------------------------*/
/*  Class Functionality                                                    */
/*-------------------------------------------------------------------------*/
#pragma region TOTUAIController.h_Class
UCLASS()
class TOTU_API ATOTUAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATOTUAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
#pragma endregion
/*-------------------------------------------------------------------------*/