// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentalInteractable.h"
#include "PlayerStateInterface.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AEnvironmentalInteractable::AEnvironmentalInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(SceneComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComponent);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComp->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void AEnvironmentalInteractable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnvironmentalInteractable::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}