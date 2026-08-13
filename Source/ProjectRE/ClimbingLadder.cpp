// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingLadder.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "kismet/GameplayStatics.h"

// Sets default values for this component's properties
UClimbingLadder::UClimbingLadder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UClimbingLadder::BeginPlay()
{
	Super::BeginPlay();
	
	OwningCharacter = Cast<ACharacter>(GetOwner());
	
	if (OwningCharacter)
	{
		MovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

// Called every frame
void UClimbingLadder::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UClimbingLadder::BeginEnvironmentInteraction_Implementation(AActor* TargetLadder, UArrowComponent* ArrowComp)
{
	CurrentLadder = TargetLadder;
	bIsClimbing = true;
	MovementComponent->StopMovementImmediately();
	MovementComponent->SetMovementMode(MOVE_Flying);
	MovementComponent->bOrientRotationToMovement = false;
	OnStartClimbing.Broadcast();
	
	const FVector MoveToLadder = ArrowComp->GetComponentLocation();
	const FRotator RotateToLadder = ArrowComp->GetComponentRotation();
	
	OwningCharacter->SetActorLocation(MoveToLadder);
	OwningCharacter->SetActorRotation(RotateToLadder);
	
	//Update Climbing Ladder Variables For Player Animation
	OnUpdateClimbingForAnimation.Broadcast(bIsClimbing);
}

void UClimbingLadder::EndEnvironmentInteraction_Implementation()
{
	bIsClimbing = false;
	MovementComponent->SetMovementMode(MOVE_Walking);
	MovementComponent->bOrientRotationToMovement = true;
	OnStopClimbing.Broadcast();
	
	//Update Climbing Ladder Variables For Player Animation
	OnUpdateClimbingForAnimation.Broadcast(bIsClimbing);
}

void UClimbingLadder::ClimbingMovement(const float Forward) const
{
	const FVector UpDirection = OwningCharacter->GetActorUpVector();
	OwningCharacter->AddMovementInput(UpDirection, Forward);
}

void UClimbingLadder::ReachedTopClimbing()
{
	if (bIsClimbing)
	{
		if (ClimbingLadderMontage)
		{
			OwningCharacter->PlayAnimMontage(ClimbingLadderMontage);
		}
		//After Finish Climbing Up Animation It Will Call To End The Climbing
		FTimerHandle EndClimbingTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(EndClimbingTimerHandle, [this]
		{
			EndEnvironmentInteraction_Implementation();
		},
		2.2,
		false
		);
		//Called To Player To Disable Input
		OnReachTopClimbing.Broadcast();
	}
}

void UClimbingLadder::FinishedTopClimbing()
{
	OnFinishedClimbing.Broadcast();
}

void UClimbingLadder::PlayClimbingLadderAudio()
{
	if (ClimbingLadderAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClimbingLadderAudio, OwningCharacter->GetActorLocation());
	}
}

