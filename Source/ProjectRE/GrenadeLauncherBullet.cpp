// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncherBullet.h"

// Sets default values
AGrenadeLauncherBullet::AGrenadeLauncherBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Component");
	SetRootComponent(RootComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComp->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void AGrenadeLauncherBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenadeLauncherBullet::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

