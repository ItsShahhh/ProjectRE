// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
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
	TObjectPtr<UBehaviorTree> BehaviorComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "AI Perception")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	
	UFUNCTION()
	void HandleTargetPerception(AActor* TargetActor, FAIStimulus Stimulus);
	
	virtual FGenericTeamId GetGenericTeamId() const override;
};
