// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestSubsystem.h"
#include "QuestObjectiveBase.generated.h"

/**
 * 
 */

class UQuestSubsystem;

UCLASS(Abstract, HideDropdown)
class PROJECTRE_API UQuestObjectiveBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UQuestSubsystem* QuestSubsystem;
	void SetQuestSubsystem(UQuestSubsystem* QuestSubsystemRef);
	
	//PURE VIRTUAL is Used For Class That Created Function It Without Has To Implement It Inside Cpp
	virtual void ProcessQuestObjective() PURE_VIRTUAL(UQuestObjectiveBase::ProcessQuestObjective,;)
	virtual bool IsCompleted() PURE_VIRTUAL(UQuestObjectiveBase::IsCompleted, return false;)
	
	UFUNCTION(BlueprintCallable)
	virtual FText GetProgressText() PURE_VIRTUAL(UQuestObjectiveBase::GetText, return FText();)
};
