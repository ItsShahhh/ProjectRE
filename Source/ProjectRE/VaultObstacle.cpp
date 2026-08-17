// Fill out your copyright notice in the Description page of Project Settings.


#include "VaultObstacle.h"

#include "ProjectRECharacter.h"

// Sets default values for this component's properties
UVaultObstacle::UVaultObstacle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UVaultObstacle::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef =  Cast<ACharacter>(GetOwner());
}

// Called every frame
void UVaultObstacle::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVaultObstacle::VaultObstacle()
{
	// Waist Trace
	// Check If Waist Trace Hit Anything
	const FVector StartLocation = GetOwner()->GetActorLocation();
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector EndLocation  = StartLocation+ (ForwardVector * LineRange);
	bHitWaist = GetWorld()->LineTraceSingleByChannel(HitObstacle, StartLocation, EndLocation, ECC_Visibility);
	
	// If Hit Something With Waist Trace We Check If Player Running or Walking/Idle
	if (bHitWaist)
	{
		OnCheckingVaultState.Broadcast();
	}
}

void UVaultObstacle::Mantle() const
{
	FVector MantleTraceStart = HitObstacle.ImpactPoint + (CharacterRef->GetActorForwardVector() * MantleDistance);
	MantleTraceStart.Z += 100.0f;
	
	FVector MantleTraceEnd = MantleTraceStart;
	// I Make The MantleTraceEnd.Z To Negative So The Line Trace is Shooting Down
	MantleTraceEnd.Z -= 100.0f;
	
	FHitResult MantleHit;
	GetWorld()->LineTraceSingleByChannel(MantleHit, MantleTraceStart, MantleTraceEnd, ECC_Visibility);
	
	FVector MantleLocation = MantleHit.Location;
	// I Added MantleLocation.Z To 100 So The Mantle Hit Location Is Above and Not Inside The Obstacle, So Player Can Mantle On Top Of The Box
	MantleLocation.Z += 100.0f;
	
	// Called bool Function To Check Whether Player Can Mantle Or Not
	if (!CanMantle()) return;
	
	OnMantleVault.Broadcast(MantleLocation, MantleAnimation);
}

void UVaultObstacle::Vaulting() const
{
	const FVector VaultTraceStart = HitObstacle.ImpactPoint + (CharacterRef->GetActorForwardVector() * VaultDistance);
	
	FVector VaultTraceEnd = VaultTraceStart;
	VaultTraceEnd.Z -= 200.0f;
	
	FHitResult VaultHit;
	GetWorld()->LineTraceSingleByChannel(VaultHit, VaultTraceStart, VaultTraceEnd, ECC_Visibility);
	
	const FVector VaultLocation = VaultHit.Location;
	
	if (!CanVault(VaultLocation)) return;
	
	// For Safety Check Because Sometime The Capsule Go Out Of Bound So It Doesn't Detect The Capsule so When Mantle Also Checking The Head Trace
	if (!CanMantle()) return;
	
	OnVault.Broadcast(VaultLocation, VaultAnimation);
}

// Making These Two Checks ("Can Vault" and "Can Mantle") Separate So It Easier To Debug Later On
bool UVaultObstacle::CanVault(FVector VaultLocation) const
{
	FHitResult CanVaultHit;
	// This is To Make Sure The Capsule Trace Doesn't Spawn On The Ground Instead The Same Height As The Player Capsule
	VaultLocation.Z += 97.0f;

	//Shooting a Sphere Trace In front Of Player To Check If There's Anything Blocking
	//If True That Mean Something Is Blocking Player From Vaulting
	const bool bHitObstacle = GetWorld()->SweepSingleByChannel(CanVaultHit, VaultLocation, VaultLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeCapsule(42, 96));
	
	if (bHitObstacle)
	{
		return false;
	}
	return true;
}

bool UVaultObstacle::CanMantle() const
{
	FVector HeadTraceStart = CharacterRef->GetActorLocation();
	// This Is To Make Sure The Head Trace Spawn Above The Belly of Player
	HeadTraceStart.Z += 36.0f;

	const FVector HeadTraceEnd = HeadTraceStart + (CharacterRef->GetActorForwardVector() * LineRange);
	
	//Shoot a Line Trace On Player Head To Check If There's Anything Blocking 
	//If True That Mean The Actor That Blocking The Trace Is Taller Than Player
	FHitResult CanMantleHit;
	const bool bHitObstacle = GetWorld()->LineTraceSingleByChannel(CanMantleHit, HeadTraceStart, HeadTraceEnd, ECC_Visibility);
	
	if (bHitObstacle)
	{
		return false;
	}
	return true;
}

void UVaultObstacle::FinishMantleVault()
{
	OnFinishVaulting.Broadcast();
	bVaulting = false;
}


