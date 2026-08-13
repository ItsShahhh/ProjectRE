// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBase.h"
#include "InventoryComponent.h"

// Sets default values
AAmmoBase::AAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmoBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAmmoBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!ItemDataAsset) return;
	
	//Save Inventory Data Struct Item So Don't Have To Set It Manually
	ItemStructs.ItemDataAsset = ItemDataAsset;
	ItemStructs.ItemQuantity= ItemDataAsset->ItemQuantity;
}

void AAmmoBase::Interactable_Implementation(AActor* InteractingActor)
{
	if (!IsValid(InteractingActor)) return;
	
	UInventoryComponent* InventoryComponent = InteractingActor->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(InventoryComponent)) return;
	
	Execute_AddPickableToInventory(InventoryComponent, this);
	Execute_PickableItemIK(InteractingActor, GetActorLocation());
	Destroy();
}

void AAmmoBase::GetInventoryDataStruct_Implementation(FInventoryData& InventoryData)
{
	 InventoryData = ItemStructs;
}

