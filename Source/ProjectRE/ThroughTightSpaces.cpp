// Fill out your copyright notice in the Description page of Project Settings.


#include "ThroughTightSpaces.h"

#include "GameplayUtils.h"
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
	if (!OwningCharacter) return;
	if (!MoveComp) return;
	if (!CurrentTightSpace) return;
	if (!ArrowComp) return;
	
	if (!bTroughTightSpaces)
	{
		bTroughTightSpaces = true;
		TightSpaceActor = CurrentTightSpace;
		ArrowComponentRef = ArrowComp;
		
		const FVector StartTightSpacesLocation = ArrowComponentRef->GetComponentLocation();
		OwningCharacter->SetActorLocation(StartTightSpacesLocation);
		
		const FRotator StartTightSpacesRotation = ArrowComponentRef->GetComponentRotation();
		OwningCharacter->SetActorRotation(StartTightSpacesRotation);
		
		const FRotator PlayerRotation = ArrowComponentRef->GetForwardVector().Rotation();
		const FRotator PLayerYawRotation = FRotator(0, -PlayerRotation.Yaw, 0);
		OwningCharacter->GetMesh()->SetWorldRotation(PLayerYawRotation);
		
		OwningCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
		MoveComp->bOrientRotationToMovement = false;	
		MoveComp->SetMovementMode(MOVE_Flying);
		MoveComp->StopMovementImmediately();
	
		StartTightSpacesDispatcher.Broadcast();
	
		//Update Through Tight Spaces Variables For Player Animation
		OnUpdateTightSpaceForAnimation.Broadcast(bTroughTightSpaces);
	}
}

void UThroughTightSpaces:: EndEnvironmentInteraction_Implementation()
{
	if (!OwningCharacter) return;
	if (!MoveComp) return;
	
	if (bTroughTightSpaces)
	{
		bTroughTightSpaces = false;
		
		OwningCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			
		UGameplayUtils::PrintDebug(1.0f, FColor::Green, TEXT("Called"), this);
		const FRotator PlayerRotation = OwningCharacter->GetMesh()->GetRightVector().Rotation();
		const FRotator PLayerYawRotation = FRotator(0, -PlayerRotation.Yaw, 0);
		OwningCharacter->GetMesh()->SetWorldRotation(PLayerYawRotation);
		
		MoveComp->bOrientRotationToMovement = true;
		MoveComp->SetMovementMode(MOVE_Walking);
		MoveComp->StopMovementImmediately();
		
		StopTightSpacesDispatcher.Broadcast();
		
		//Update Through Tight Spaces Variables For Player Animation
		OnUpdateTightSpaceForAnimation.Broadcast(bTroughTightSpaces);
	}
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

