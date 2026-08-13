// Fill out your copyright notice in the Description page of Project Settings.


#include "KillQuestObjective.h"
#include "QuestSubsystem.h"

void UKillQuestObjective::ProcessQuestObjective()
{
	if (CurrentKill <= KillRequired)
	{
		CurrentKill++;
		QuestSubsystem->OnUpdateProgressQuest.Broadcast(GetProgressText());
	}
}

bool UKillQuestObjective::IsCompleted()
{
	return CurrentKill == KillRequired;
}

FText UKillQuestObjective::GetProgressText()
{
	const FText KillCounterFormat = FText::FromString(QuestProgressText.ToString());
	FText KillCounterFinal = FText::Format(KillCounterFormat, CurrentKill, KillRequired);
	
	return KillCounterFinal;
}