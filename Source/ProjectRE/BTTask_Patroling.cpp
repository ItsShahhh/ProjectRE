// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patroling.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Patrolling::UBTTask_Patrolling()
{
	NodeName = "Patrolling";
}

EBTNodeResult::Type UBTTask_Patrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	const APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const FVector EnemyLocation = Pawn->GetActorLocation();

	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation ResultLocation;
		constexpr float SearchRadius = 1500.0f;

		if (NavSys->GetRandomReachablePointInRadius(EnemyLocation, SearchRadius, ResultLocation))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ResultLocation.Location);
          
			// FIX 2: Only return Succeeded if we ACTUALLY found a location and saved it!
			return EBTNodeResult::Succeeded;
		}
	}
    
	// If NavSys didn't exist, or Success failed, tell the tree we failed so it can try again
	return EBTNodeResult::Failed;
}
