// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SplinePath.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SplineComponent.h"

UBTTask_SplinePath::UBTTask_SplinePath()
{
	NodeName = "Move To Spline Path";
}

EBTNodeResult::Type UBTTask_SplinePath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	const AAIController* AIComp = OwnerComp.GetAIOwner();
	if (!AIComp) return EBTNodeResult::Failed;

	const APawn* Pawn = AIComp->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const AActor* SplineRef = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("SplinePath")));
	if (!SplineRef) return EBTNodeResult::Failed;

	const USplineComponent* SplineComp = SplineRef->FindComponentByClass<USplineComponent>();
	if (!SplineComp) return EBTNodeResult::Failed;
	
	int32 CurrentSplineIndex = BlackboardComp->GetValueAsInt(FName("SplineIndex"));
	const int32 NumberOfPoints = SplineComp->GetNumberOfSplinePoints();
	
	FVector SplineLocation = SplineComp->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	BlackboardComp->SetValueAsVector(FName("SplineLocation"), SplineLocation);
	const float DistanceToTarget = FVector::Dist(Pawn->GetActorLocation(), SplineLocation);
	
	if (DistanceToTarget <= 100.0f)
	{
		CurrentSplineIndex++;
		
		if (CurrentSplineIndex >= NumberOfPoints)
		{
			CurrentSplineIndex = 0;
		}
		SplineLocation = SplineComp->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	}
	BlackboardComp->SetValueAsVector(FName("SplineLocation"), SplineLocation);
	BlackboardComp->SetValueAsInt(FName("SplineIndex"), CurrentSplineIndex);
    
	return EBTNodeResult::Succeeded;
}
