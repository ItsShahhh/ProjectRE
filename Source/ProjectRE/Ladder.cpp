// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "ClimbingLadder.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

ALadder::ALadder()
{
	TopLadderCollision = CreateDefaultSubobject<UBoxComponent>("TopLadderCollision");
	TopLadderCollision->SetupAttachment(SceneComponent);
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
	TopLadderCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::ReachTopLadderOverlap);
	
	CharacterRef = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	if (CharacterRef)
	{
		ClimbingLadderRef = CharacterRef->FindComponentByClass<UClimbingLadder>();
	}
}

void ALadder::ReachTopLadderOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ClimbingLadderRef->ReachedTopClimbing();
}

void ALadder::Interactable_Implementation(AActor* /*InteractingActor*/)
{
	if (!IsValid(CharacterRef)) return;
	if (!IsValid(ClimbingLadderRef)) return;
	if (!ClimbingLadderRef->Implements<UInteractableInterface>()) return;
	
	if (!ClimbingLadderRef->GetIsClimbing())
	{
		IInteractableInterface::Execute_BeginEnvironmentInteraction(ClimbingLadderRef, this, ArrowComp);
	}
	else if (ClimbingLadderRef->GetIsClimbing())
	{
		Execute_EndEnvironmentInteraction(ClimbingLadderRef);
	}
}


