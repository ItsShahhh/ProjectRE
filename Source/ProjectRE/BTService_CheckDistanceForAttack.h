// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckDistanceForAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRE_API UBTService_CheckDistanceForAttack : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit  UBTService_CheckDistanceForAttack();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere)
	float AttackRange = 200.0f;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InRangeKey;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PlayerRefKey;
	
};
