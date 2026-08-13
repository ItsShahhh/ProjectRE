// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyShell.h"

// Sets default values
AEmptyShell::AEmptyShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
}

// Called when the game starts or when spawned
void AEmptyShell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEmptyShell::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

