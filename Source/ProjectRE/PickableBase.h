// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "InventoryDataStruct.h"
#include "PickableBase.generated.h"

UCLASS()
class PROJECTRE_API APickableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<UItemDataAsset> ItemDataAsset;
	
	UPROPERTY(EditAnywhere, Category = "Struct")
	FInventoryData ItemStructs;
};
