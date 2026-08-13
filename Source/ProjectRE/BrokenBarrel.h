// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrokenBarrel.generated.h"

UCLASS()
class PROJECTRE_API ABrokenBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrokenBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankOne;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankTwo;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankThree;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankFour;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankFive;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlankSix;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
