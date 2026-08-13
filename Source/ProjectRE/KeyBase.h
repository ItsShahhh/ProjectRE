// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "KeyBase.generated.h"

class AProjectReCharacter;

UCLASS()
class PROJECTRE_API AKeyBase : public APickableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyBase();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void  Interactable_Implementation(AActor* InteractingActor) override;
	virtual void GetInventoryDataStruct_Implementation(FInventoryData& InventoryData) override;
	
};
