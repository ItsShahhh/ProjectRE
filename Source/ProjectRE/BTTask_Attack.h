// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

class FOnAttackFinishedSignature;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class PROJECTRE_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	APawn* EnemyActor;
	
	UPROPERTY()
	AAIController* AIController;
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComponent;
	
	IEnemyInterface* EnemyInterface;
	
	UBTTask_Attack();
	
public:
	UFUNCTION()
	void OnAnimationFinished();
	
	FOnAttackFinishedSignature* AttackFinishDelegate;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
