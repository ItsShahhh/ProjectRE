// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SplinePath.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRE_API UBTTask_SplinePath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	explicit UBTTask_SplinePath();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
