// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRE_API IPlayerStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void DoorUnlockMode(AActor* DoorCamera);
	
	UFUNCTION(BlueprintNativeEvent)
	void InteractWith(AActor* InteractingActor);
	
	UFUNCTION(BlueprintNativeEvent)
	void CorrectDoorKey(FGameplayTag CorrectDoorTag);
	
	UFUNCTION(BlueprintNativeEvent)
	void CameraWeaponRecoil(float CurrentRecoilWeapon);
};
