// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "EnemyInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "Attack Animation";
	bNotifyTick = false;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComponent = &OwnerComp;
	if (!CachedOwnerComponent) return EBTNodeResult::Failed;
	
	AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return EBTNodeResult::Failed;
	
	EnemyActor = AIController->GetPawn();
	if (!EnemyActor) return EBTNodeResult::Failed;
	
	EnemyInterface = Cast<IEnemyInterface>(EnemyActor);
	if (!EnemyInterface) return EBTNodeResult::Failed;
	
	AttackFinishDelegate = EnemyInterface->GetAttackFinishedDelegate();
	if (!AttackFinishDelegate) return EBTNodeResult::Failed;
	
	if (!EnemyActor->Implements<UEnemyInterface>()) return EBTNodeResult::Failed;
	
	//Bind This Delegate On Animation Finished
	AttackFinishDelegate->AddDynamic(this, &UBTTask_Attack::OnAnimationFinished);
	
	//Execute Enemy Attack On Enemy Code
	IEnemyInterface::Execute_EnemyAttack(EnemyActor);
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnAnimationFinished()
{
	FinishLatentTask(*CachedOwnerComponent, EBTNodeResult::Succeeded);
	
	//Unbind This Delegate From On Animation Finished
	AttackFinishDelegate->RemoveDynamic(this, &UBTTask_Attack::OnAnimationFinished);
	
}


