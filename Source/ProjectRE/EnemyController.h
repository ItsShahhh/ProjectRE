// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	UAIPerceptionComponent* AIPerceptionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	class UAISenseConfig_Hearing* HearingConfig;
	
	UFUNCTION()
	void HandleTargetPerception(AActor* TargetActor, FAIStimulus Stimulus);
	
	virtual FGenericTeamId GetGenericTeamId() const override;
};
