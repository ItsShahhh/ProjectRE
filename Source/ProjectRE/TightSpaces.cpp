// Fill out your copyright notice in the Description page of Project Settings.


#include "TightSpaces.h"
#include "ThroughTightSpaces.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ATightSpaces::ATightSpaces()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SceneComponent);
	
	EndTightSpaces = CreateDefaultSubobject<UBoxComponent>("EndTightSpaces");
	EndTightSpaces->SetupAttachment(SceneComponent);
}

void ATightSpaces::BeginPlay()
{
	Super::BeginPlay();
	
	EndTightSpaces->OnComponentBeginOverlap.AddDynamic(this, &ATightSpaces::ReachEndOverlapBegin);
	
	CharacterRef = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	if (IsValid(CharacterRef))
	{
		TightSpacesRef = CharacterRef->FindComponentByClass<UThroughTightSpaces>();
	}
}

void ATightSpaces::Interactable_Implementation(AActor* /*InteractingActor*/)
{
	if (!IsValid(CharacterRef)) return;
	if (!IsValid(TightSpacesRef)) return;
	if (!TightSpacesRef->Implements<UInteractableInterface>()) return;
	
	if (!TightSpacesRef->GetIsThroughTightSpaces())
	{
		Execute_BeginEnvironmentInteraction(TightSpacesRef, this, ArrowComp);
	}
	else
	{
		Execute_EndEnvironmentInteraction(TightSpacesRef);
	}
}

void ATightSpaces::ReachEndOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TightSpacesRef)
	{
		Execute_EndEnvironmentInteraction(TightSpacesRef);
	}
}
