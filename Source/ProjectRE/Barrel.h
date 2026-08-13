// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageableInterface.h"
#include "Barrel.generated.h"

class UArrowComponent;
class AAmmoBase;

UCLASS()
class PROJECTRE_API ABarrel : public AActor, public IDamageableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BarrelMesh;
	
	UPROPERTY(EditAnywhere, Category= "Barrel")
	TMap<int32, TSubclassOf<AAmmoBase>>AmmoSpawnActor;
	
	UPROPERTY(EditAnywhere, Category= "Barrel")
	TSubclassOf<AActor>BrokenBarrelActor;
	
	UPROPERTY(EditAnywhere, Category= "Barrel")
	float AmmoSpawnHeight = 20.0f;
	
	UPROPERTY(EditAnywhere, Category= "Barrel")
	float BrokenBarrelLifetime = 3.0f;
	
private:
	UPROPERTY()
	AActor* BrokenBarrelSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnReceiveImpact_Implementation() override;
};
