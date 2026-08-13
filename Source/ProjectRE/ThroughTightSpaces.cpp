// Fill out your copyright notice in the Description page of Project Settings.


#include "ThroughTightSpaces.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UThroughTightSpaces::UThroughTightSpaces()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UThroughTightSpaces::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	MoveComp = OwningCharacter->GetCharacterMovement();
}

// Called every frame
void UThroughTightSpaces::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UThroughTightSpaces::BeginEnvironmentInteraction_Implementation(AActor* CurrentTightSpace, UArrowComponent* ArrowComp)
{
	bTroughTightSpaces = true;
	TightSpaceActor = CurrentTightSpace;
	
	if (OwningCharacter)
	{
		const FVector StartTightSpacesLocation = ArrowComp->GetComponentLocation();
		const FRotator StartTightSpacesRotation = ArrowComp->GetComponentRotation();
		OwningCharacter->SetActorLocation(StartTightSpacesLocation);
		OwningCharacter->SetActorRotation(StartTightSpacesRotation);
		OwningCharacter->GetMesh()->SetWorldRotation(FRotator(0, -90, 0));
		OwningCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		
		if (MoveComp)
		{
			MoveComp->bOrientRotationToMovement = false;	
			MoveComp->SetMovementMode(MOVE_Flying);
			MoveComp->StopMovementImmediately();
		}
		StartTightSpacesDispatcher.Broadcast();
	}
	//Update Through Tight Spaces Variables For Player Animation
	OnUpdateTightSpaceForAnimation.Broadcast(bTroughTightSpaces);
}

void UThroughTightSpaces:: EndEnvironmentInteraction_Implementation()
{
	if (bTroughTightSpaces)
	{
		bTroughTightSpaces = false;
		
		if (OwningCharacter)
		{
			OwningCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			OwningCharacter->GetMesh()->SetWorldRotation(FRotator(0, 180, 0));
		
			if (MoveComp)
			{
				MoveComp->bOrientRotationToMovement = true;
				MoveComp->SetMovementMode(MOVE_Walking);
				MoveComp->StopMovementImmediately();
			}
			StopTightSpacesDispatcher.Broadcast();
		}
	}
	//Update Through Tight Spaces Variables For Player Animation
	OnUpdateTightSpaceForAnimation.Broadcast(bTroughTightSpaces);
}

void UThroughTightSpaces::TightSpacesMovement(const float Forward) const
{
	const float ForwardOnly = FMath::Max(0.0f, Forward);
	const FVector FrontDirection = OwningCharacter->GetActorForwardVector();
	
	if (OwningCharacter)
	{
		OwningCharacter->AddMovementInput(FrontDirection, ForwardOnly);
	}
}

