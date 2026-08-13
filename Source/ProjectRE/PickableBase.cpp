// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableBase.h"

#include "InventoryComponent.h"

class UInventoryComponent;
// Sets default values
APickableBase::APickableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APickableBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickableBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}