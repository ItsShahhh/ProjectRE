// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWeaponTypeBase.h"
#include "PickableBase.h"
#include "AmmoBase.generated.h"

class AProjectReCharacter;
class UInventoryComponent;

UCLASS()
class PROJECTRE_API AAmmoBase : public APickableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBase();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType AmmoType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AmmoSize;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void  Interactable_Implementation(AActor* InteractingActor) override;
	
	virtual void GetInventoryDataStruct_Implementation(FInventoryData& InventoryData) override;
	
	FORCEINLINE EWeaponType GetAmmoType() const {return AmmoType;}
	
	FORCEINLINE int32 GetAmmoSize() const {return AmmoSize;}
	
	virtual void Tick(float DeltaTime) override;
};
