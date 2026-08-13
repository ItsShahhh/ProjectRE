// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDistanceForAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckDistanceForAttack::UBTService_CheckDistanceForAttack()
{
	NodeName = "Check Distance For Attack";
	bNotifyTick = true;
}

void UBTService_CheckDistanceForAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	const float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return;

	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	const APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return;

	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(PlayerRefKey.SelectedKeyName));
	if (!TargetActor) return;
	
	// Calculate Between Enemy and Player Location For Attack
	if (const float Distance = FVector::Dist(Pawn->GetActorLocation(), TargetActor->GetActorLocation()); Distance <= AttackRange)
	{
		// Setting Selected Key (InRangeKey For Now) to Whatever Value Based On BB key
		BlackboardComponent->SetValueAsBool(InRangeKey.SelectedKeyName, true);
	}
	else if (Distance >= AttackRange)
	{
		BlackboardComponent->SetValueAsBool(InRangeKey.SelectedKeyName, false);
	}
}