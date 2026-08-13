// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

class UQuestObjectiveBase;
/**
 * 
 */
UCLASS()
class PROJECTRE_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag QuestTag;
	
	UPROPERTY(EditAnywhere,Instanced, BlueprintReadOnly)
	TObjectPtr<UQuestObjectiveBase> QuestObjectives;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UQuestData> NextQuest;
	
};
