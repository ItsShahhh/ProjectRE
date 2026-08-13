// Fill out your copyright notice in the Description page of Project Settings.


#include "UnlockDoor.h"

#include "GameplayUtils.h"
#include "InventoryComponent.h"
#include "PlayerStateInterface.h"
#include "QuestSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

AUnlockDoor::AUnlockDoor()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SceneComponent);
}

void AUnlockDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
			if (UQuestSubsystem* QuestSubsystemRef = GameInstance->GetSubsystem<UQuestSubsystem>())
			{
				QuestSubsystem = QuestSubsystemRef;
			}
	}
}

// Interacting Actor Is Actor That Interact With This Actor
void AUnlockDoor::Interactable_Implementation(AActor* InteractingActor)
{
	//Check If Door Is Already Open, If True Player Can't Interact With It Again
	if (IsAlreadyOpen) return;
	
	PlayerRef = InteractingActor;
	
	if (!PlayerRef) return;
	
	if (PlayerRef->Implements<UPlayerStateInterface>())
	{
		IPlayerStateInterface::Execute_DoorUnlockMode(PlayerRef, this);
	}
}

void AUnlockDoor::CheckDoorKeys_Implementation(const FGameplayTag CorrectKeyTag)
{
	if (CorrectKeyTag.MatchesTagExact(CorrectTag))
	{
		if (UInventoryComponent* InventoryComp = PlayerRef->FindComponentByClass<UInventoryComponent>())
		{
			IPlayerStateInterface::Execute_CorrectDoorKey(InventoryComp, CorrectTag);
			
			if (IsValid(QuestSubsystem) && FindUnlockQuestTag.IsValid())
			{
				QuestSubsystem->ReportGameplayEvent(FindUnlockQuestTag);
			}
			else
			{
				UGameplayUtils::PrintDebug(1.0f, FColor::Red, TEXT(": Quest Subsystem or Find Unlock Quest Is Null"), this);
			}
			
			//Enable Door Open Physics 
			StaticMesh->SetSimulatePhysics(true);
			
			//Play Correct Door Sound
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), CorrectDoorSound, GetActorLocation());
			
			IsAlreadyOpen = true;
		}
	}
	else
	{
		//Play Incorrect Door Sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), IncorrectDoorSound, GetActorLocation());
	}
}

