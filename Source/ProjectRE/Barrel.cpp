// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"

#include "AmmoBase.h"
#include "GameplayUtils.h"

// Sets default values
ABarrel::ABarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<UStaticMeshComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelMesh");
	BarrelMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ABarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABarrel::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarrel::OnReceiveImpact_Implementation()
{
	if (AmmoSpawnActor.IsEmpty())
	{
		UGameplayUtils::PrintDebug(5.0f, FColor::Red, TEXT("TMap For Ammo Spawn Is Null"), this);
		return;
	}
	
	if (!BrokenBarrelActor)
	{
		UGameplayUtils::PrintDebug(5.0f, FColor::Red, TEXT("Broken Barrel Actor Is Null"), this);
		return;
	}
	
	// Set The Barrel Visibility and Collision Off Instead Of Destroy It Because Of Timer
	BarrelMesh->SetVisibility(false);
	BarrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//Spawn The Ammo Upward So It's Not Going To Spawn Underground
	const FVector BarrelLocation = GetActorLocation();
	const float BarrelZ = BarrelLocation.Z + AmmoSpawnHeight;
	const FVector AmmoSpawnUp = FVector(BarrelLocation.X, BarrelLocation.Y, BarrelZ);
	
	//Randomize Between Min to Max Number Of TMap
	const int32 RandomInt = FMath::RandRange(1, AmmoSpawnActor.Num());
	
	//Using The Number That's Been Randomized It Will Set The TSubClass Of The Ammo  
	const TSubclassOf<AAmmoBase> SelectedAmmo = AmmoSpawnActor[RandomInt];
	
	//Spawn Ammo Based On Ammo Spawn Scene Component
	GetWorld()->SpawnActor<AAmmoBase>(SelectedAmmo,AmmoSpawnUp, GetActorRotation(), SpawnParams);
	
	//Spawn The Broken Barrel Actors
	BrokenBarrelSpawn = GetWorld()->SpawnActor<AActor>(BrokenBarrelActor, GetActorLocation(), GetActorRotation(), SpawnParams);
	
	//Start Timer To Destroy Broken Plank and Barrel
	FTimerHandle BrokenBarrelTimerHandle;
	GetWorldTimerManager().SetTimer(BrokenBarrelTimerHandle, [this]
	{
		//Destroy Both Broken Barrel and Barrel When Timer Is Up
		BrokenBarrelSpawn->Destroy();
		Destroy();
	},
	BrokenBarrelLifetime,
	false
	);
}