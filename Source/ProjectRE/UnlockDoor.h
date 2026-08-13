// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentalInteractable.h"
#include "GameplayTagContainer.h"
#include "LockableInterfaces.h"
#include "UnlockDoor.generated.h"

class UQuestSubsystem;
class UHotbarData;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECTRE_API AUnlockDoor : public AEnvironmentalInteractable, public ILockableInterfaces
{
	GENERATED_BODY()
	
	AUnlockDoor();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere, Category= "Keys")
	FGameplayTag CorrectTag;
	
	UPROPERTY(EditDefaultsOnly, Category= "Quest")
	FGameplayTag FindUnlockQuestTag;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sound")
	USoundBase* CorrectDoorSound;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sound")
	USoundBase* IncorrectDoorSound;
	
	UPROPERTY(VisibleAnywhere)
	bool IsAlreadyOpen = false;

private:
	UPROPERTY()
	UQuestSubsystem* QuestSubsystem;
	
	UPROPERTY()
	AActor* PlayerRef;
	
public:
	virtual void Interactable_Implementation(AActor* InteractingActor) override;
	virtual void CheckDoorKeys_Implementation(FGameplayTag CorrectKeyTag) override;

};

