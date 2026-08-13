// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubsystem.h"

#include "GameplayUtils.h"
#include "QuestData.h"
#include "QuestObjectiveBase.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UQuestSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UQuestSubsystem::StartQuest(UQuestData* ReceiveQuest)
{
	if (!IsValid(ReceiveQuest)) return;
	
	//Set The Struct Quest Rule To Receive Quest Data So We Can Change The Value Without Bothering Original Data
	ActiveQuestsTracker.QuestRules = ReceiveQuest;
	
	UQuestObjectiveBase* DuplicateObjective = DuplicateObject<UQuestObjectiveBase>(ReceiveQuest->QuestObjectives, this);
	
	if (!IsValid(DuplicateObjective)) return;
	ActiveQuestsTracker.ObjectiveQuests = DuplicateObjective;
	
	//Check If It Already Set Or Not
	if (!IsAlreadySet)
	{
		//Give Objective Base This Subsystem Reference To Use For Updating UI
		ActiveQuestsTracker.ObjectiveQuests->SetQuestSubsystem(this);
		IsAlreadySet = true;
	}
	
	OnUpdateProgressQuest.Broadcast(ActiveQuestsTracker.ObjectiveQuests->GetProgressText());
	OnUpdateQuestLeaderboard.Broadcast(ActiveQuestsTracker.QuestRules->QuestName, ActiveQuestsTracker.QuestRules->QuestDescription);
}

void UQuestSubsystem::ReportGameplayEvent(const FGameplayTag ReportObjectiveTag)
{
	if (!IsValid(ActiveQuestsTracker.ObjectiveQuests))
	{
		UGameplayUtils::PrintDebug(5.0f, FColor::Red, TEXT("No Quest"), this);
		return;
	}
	
	//Check If The Report Tag Is The Same As The Current Active Quest (It Get Confused When Player Kill Zombie Even Though It's Not The Quest)
	if (!ReportObjectiveTag.MatchesTag(ActiveQuestsTracker.QuestRules->QuestTag))
	{
		UGameplayUtils::PrintDebug(5.0f, FColor::Red, TEXT("Not For This Quest"), this);
		return;
	}
	
	//Called Process Quest Based On Selected Objective Quest
	ActiveQuestsTracker.ObjectiveQuests->ProcessQuestObjective();
	
	//After Process It Check If The Current Quest Is Completed
	CheckCompleteQuest();
}

void UQuestSubsystem::CheckCompleteQuest()
{
	if (!IsValid( ActiveQuestsTracker.ObjectiveQuests))
	{
		UGameplayUtils::PrintDebug(5.0f, FColor::Red, TEXT("No Quest"), this);
		return;
	}
	
	UQuestData* NextQuest = ActiveQuestsTracker.QuestRules->NextQuest;
	
	if (ActiveQuestsTracker.ObjectiveQuests->IsCompleted())
	{
		if (IsValid(NextQuest))
		{
			StartQuest(NextQuest);
		}
		else 
		{
			UGameplayUtils::PrintDebug(5.0f, FColor::Green, TEXT("Quest Chained Completed"), this);
		}
	}
}
