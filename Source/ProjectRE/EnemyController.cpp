// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyController::AEnemyController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception Component");
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	
	if (SightConfig)
	{
		SightConfig->SightRadius = 700.0f;
		SightConfig->LoseSightRadius = 900.0f;
		SightConfig->PeripheralVisionAngleDegrees = 45.0f;
		SightConfig->SetMaxAge(1.0f);
		
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerceptionComp->ConfigureSense(*SightConfig);
	}
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing Config");
	
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 1000.0f;
		HearingConfig->SetMaxAge(1.0f);
		
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerceptionComp->ConfigureSense(*HearingConfig);
	}
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorComp)
	{
		RunBehaviorTree(BehaviorComp);
	}
	
	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::HandleTargetPerception);
	}
}

void AEnemyController::HandleTargetPerception(AActor* TargetActor, FAIStimulus Stimulus)
{
	if (!TargetActor || !GetBlackboardComponent()) return;
	
	const FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
	const FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();
	
	if (Stimulus.Type == SightID)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(FName("PlayerTarget"), TargetActor);
			// Getting Player Reference To Use For Checking Distance (Or Can use It For Later On) 
			GetBlackboardComponent()->SetValueAsObject(FName("PlayerRef"), TargetActor);
			GetBlackboardComponent()->ClearValue(FName("LastKnownLocation"));
		}
		else
		{
			// Clear Player Target Value To Make Sure Enemy Doesn't Chase Player Anymore (Act Like Enemy Doesn't See Player Anymore)
			GetBlackboardComponent()->ClearValue(FName("PlayerTarget"));
		}
	}
	else if (Stimulus.Type == HearingID)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsVector(FName("LastKnownLocation"), TargetActor->GetActorLocation());
		}
	}
}

FGenericTeamId AEnemyController::GetGenericTeamId() const
{
if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(GetPawn()))
{
	return TeamAgent->GetGenericTeamId();
}
	return FGenericTeamId(255);
}

