// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "QuestObjectiveBase.h"
#include "KillQuestObjective.generated.h"

/**
 * 
 */

UCLASS(DisplayName = "Kill Enemies (Integer)",EditInlineNew)
class PROJECTRE_API UKillQuestObjective : public UQuestObjectiveBase
{
	GENERATED_BODY()
	
protected:
	//Remember To Put {} If The Text Has Format Function For The Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestProgressText = FText::FromString(TEXT(""));
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 KillRequired = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentKill = 0;
	
public:
	virtual void ProcessQuestObjective() override;
	virtual bool IsCompleted() override;
	virtual FText GetProgressText() override;
};
