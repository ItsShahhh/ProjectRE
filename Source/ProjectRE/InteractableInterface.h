// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryDataStruct.h"
#include "InteractableInterface.generated.h"

struct FInventoryData;
class UInventoryDataStruct;
class UItemDataAsset;
class AProjectReCharacter;
class AWeaponBase;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTRE_API IInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interactable(AActor* InteractingActor);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void BeginEnvironmentInteraction(AActor* InteractingActor, UArrowComponent* ArrowComponent);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void EndEnvironmentInteraction();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void AddWeaponToInventory(AWeaponBase* PickedUpWeapon, AActor* PickableActor);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void AddPickableToInventory(AActor* PickableActor);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void GetInventoryDataStruct(FInventoryData& InventoryData);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void PickableItemIK(const FVector& PickableItemLocation);
};
