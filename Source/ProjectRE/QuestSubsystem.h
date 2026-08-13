// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

struct FGameplayTag;
class UQuestObjectiveBase;
class UQuestData;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateQuestLeaderboard, FText, QuestName, FText, QuestDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateProgressQuest, FText, ProgressString);

USTRUCT(BlueprintType)
struct FActiveQuestTracker
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestData* QuestRules;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UQuestObjectiveBase* ObjectiveQuests;
};

UCLASS()
class PROJECTRE_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	//For Making Sure Only Set Subsystem Once For Quest UObject
	bool IsAlreadySet = false;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	void StartQuest(UQuestData* ReceiveQuest);
	UFUNCTION()
	void ReportGameplayEvent(FGameplayTag ReportObjectiveTag);
	UFUNCTION()
	void CheckCompleteQuest();
	
	UPROPERTY(VisibleAnywhere)
	FActiveQuestTracker ActiveQuestsTracker;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateQuestLeaderboard OnUpdateQuestLeaderboard;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateProgressQuest OnUpdateProgressQuest;
};
