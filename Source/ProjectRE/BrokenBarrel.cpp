// Fill out your copyright notice in the Description page of Project Settings.


#include "BrokenBarrel.h"

// Sets default values
ABrokenBarrel::ABrokenBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<UStaticMeshComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	PlankOne = CreateDefaultSubobject<UStaticMeshComponent>("PlankOne");
	PlankOne->SetupAttachment(RootComp);
	
	PlankTwo = CreateDefaultSubobject<UStaticMeshComponent>("PlankTwo");
	PlankTwo->SetupAttachment(RootComp);
	
	PlankThree = CreateDefaultSubobject<UStaticMeshComponent>("PlankThree");
	PlankThree->SetupAttachment(RootComp);
	
	PlankFour = CreateDefaultSubobject<UStaticMeshComponent>("PlankFour");
	PlankFour->SetupAttachment(RootComp);
	
	PlankFive = CreateDefaultSubobject<UStaticMeshComponent>("PlankFive");
	PlankFive->SetupAttachment(RootComp);
	
	PlankSix = CreateDefaultSubobject<UStaticMeshComponent>("PlankSix");
	PlankSix->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void ABrokenBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrokenBarrel::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

