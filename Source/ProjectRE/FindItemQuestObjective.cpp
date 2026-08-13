// Fill out your copyright notice in the Description page of Project Settings.


#include "FindItemQuestObjective.h"

void UFindItemQuestObjective::ProcessQuestObjective()
{
	bFoundItem = true;
}

bool UFindItemQuestObjective::IsCompleted()
{
	return bFoundItem;
}

FText UFindItemQuestObjective::GetProgressText()
{
	const FText ItemQuestProgress = bFoundItem ? FText::FromString("Completed") : FText::FromString("Incompleted");
	const FText ItemQuestFormat = FText::FromString(QuestProgressText.ToString());
	FText ItemQuestFinal = FText::Format(ItemQuestFormat, ItemQuestProgress);
	
	return ItemQuestFinal;
}
