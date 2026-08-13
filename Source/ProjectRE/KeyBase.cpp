// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyBase.h"

#include "InventoryComponent.h"

class UInventoryComponent;
// Sets default values
AKeyBase::AKeyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKeyBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKeyBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKeyBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!ItemDataAsset) return;
	
	//Save Inventory Data Struct Item So Don't Have To Set It Manually
	ItemStructs.ItemDataAsset = ItemDataAsset;
	ItemStructs.ItemQuantity= ItemDataAsset->ItemQuantity;
}

void AKeyBase:: Interactable_Implementation(AActor* InteractingActor)
{
	if (UInventoryComponent* InventoryComponent = InteractingActor->FindComponentByClass<UInventoryComponent>())
	{
		Execute_AddPickableToInventory(InventoryComponent, this);
	}
	
	Execute_PickableItemIK(InteractingActor, GetActorLocation());
	
	Destroy();
}

void AKeyBase::GetInventoryDataStruct_Implementation(FInventoryData& InventoryData) 
{
	InventoryData = ItemStructs;
}

