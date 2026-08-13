// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "DiedInterface.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

// Called every frame
void UHealth::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealth::OnReceiveDamage_Implementation(const float ReceiveDamage)
{
	if (!bIsAlive) return; 
	
	//Subtract Current Health With Receive Damage
	CurrentHealth -= ReceiveDamage;
	
	if (CurrentHealth <= 0.0f)
	{
		bIsAlive = false;
		
		// Execute Interface On Any Script that Implemented This Interface
		IDiedInterface::Execute_Died(GetOwner());
	}
	// Delegate To Update Player Health Bar UI (When Damage Enemy It Will Also Call This But We Didn't Bind This Event On The Enemy So Nothing Will Happen)
	OnHealthChanged.Broadcast(CurrentHealth, bIsAlive);
}