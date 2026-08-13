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
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankOne;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankTwo;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankThree;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankFour;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankFive;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PlankSix;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
