// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableTrace.h"
#include "DrawDebugHelpers.h"
#include "InteractableInterface.h"

// Sets default values for this component's properties
UInteractableTrace::UInteractableTrace()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInteractableTrace::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UInteractableTrace::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableTrace::TraceTimerScanner(const FVector& StartTraceLocation, const FVector& ForwardTraceLocation)
{
	const FVector EndCameraLocation = StartTraceLocation + (ForwardTraceLocation * LineTraceRange);
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTraceLocation, EndCameraLocation, ECC_Visibility))
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor->Implements<UInteractableInterface>())
			{
				if (!IsFacingInteractable(HitActor)) return;
				
				//Called an Interactable Interface On The Actor That We Interact
				IInteractableInterface::Execute_Interactable(HitActor, GetOwner());
			}
		}
	}
	//Draw Line Trace Debug
	DrawDebugLine(GetWorld(), StartTraceLocation, EndCameraLocation, FColor::Yellow, false, .1, 0 , 1);
}

bool UInteractableTrace::IsFacingInteractable(const AActor* ItemActor) const
{
	const FVector PlayerLocation = GetOwner()->GetActorLocation();
	const FVector PlayerForward = GetOwner()->GetActorForwardVector();
	const FVector ItemLocation = ItemActor->GetActorLocation();
	
	// We Subtract The Interact Item and Player Location To Get An Arrow That Pointing Toward Player, 
	// Then We Use Safe Normal To Make The FVector Value To Just 1.
	const FVector DirectionToTarget = (ItemLocation - PlayerLocation).GetSafeNormal();
	
	// Using Player Forward Vector and Direction To Target We Can Determine Whether Player Is Looking,
	//Directly At Interact Item Or Not
	const float DotResult = FVector::DotProduct(PlayerForward, DirectionToTarget);
	
	if (DotResult > 0.6f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Facing Target: %f"), DotResult));
		return true;
	}
	
	if (DotResult < 0.5f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not Facing Target: %f"), DotResult));
		return false;
	}
	// Default Value
	return false;
}




