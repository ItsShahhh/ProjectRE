// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveBase.h"
#include "FindItemQuestObjective.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Find Item (Boolean)",EditInlineNew)
class PROJECTRE_API UFindItemQuestObjective : public UQuestObjectiveBase
{
	GENERATED_BODY()
	
protected:
	//Remember To Put {} If The Text Has Format Function For The Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText QuestProgressText = FText::FromString(TEXT(""));
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFoundItem = false;
	
public:
	virtual void ProcessQuestObjective() override;
	virtual bool IsCompleted() override;
	virtual FText GetProgressText() override;
};
