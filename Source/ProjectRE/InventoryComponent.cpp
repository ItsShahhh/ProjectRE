// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "GameplayUtils.h"
#include "ItemDataAsset.h"
#include "WeaponBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OnMoveItemHotbar.AddDynamic(this, &UInventoryComponent::MoveWeaponHotbar);
	
	//Set The Number Of Hotbar Array At The Start Of The Game
	HotbarWeapons.SetNum(NumberOfHotbar);
	
}

// Called every frame
void UInventoryComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

#pragma region Weapon Hotbar
void UInventoryComponent::AddWeaponToInventory_Implementation(AWeaponBase* PickedUpWeapon, AActor* PickableActor)
{
	if (!PickedUpWeapon) return;
	if (!PickableActor) return;
	
	const FName WeaponSocket = PickedUpWeapon->GetWeaponSocketName();
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
	const TSubclassOf<AWeaponBase>WeaponToSpawn = PickedUpWeapon->GetClass();
		
	if (AWeaponBase* SpawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams))
	{
		//Set Actor and Collision Into False After Spawn The Weapon
		SpawnWeapon->SetActorHiddenInGame(true);
		SpawnWeapon->SetActorEnableCollision(false);
			
		if (const ACharacter* CharacterRef = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			//Call This To Check If Hotbar Is Still Available For Spawn Weapon
			CheckAddItemToHotbar(SpawnWeapon);
				
			// Attach Spawn Gun Into Player Hand After Pickup
			SpawnWeapon->AttachToComponent(CharacterRef->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		}
	}
	
	//Getting The Pickable Actor Struct and Caching It Inside Inventory Data Struct
	Execute_GetInventoryDataStruct(PickableActor, InventoryDataStruct);
	
	//Add Weapon Into Hotbar UI
	OnUpdateHotbar.Broadcast(InventoryDataStruct.ItemDataAsset->ItemIcon, InventoryDataStruct.ItemDataAsset->ItemName);
}

bool UInventoryComponent::CheckEquipWeapon(int32 WeaponSlot)
{
	//Subtract 1 Because Array Start With 0
	WeaponSlot -= 1;
	
	if (!HotbarWeapons.IsValidIndex(WeaponSlot))
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Nothing Contain On That Array Slot"),this);
		return false;
	}
	
	//Set The Selected Weapon Based On The Hotbar Weapon Array Index
	AWeaponBase* SelectedWeapon = HotbarWeapons[WeaponSlot];
	if (!SelectedWeapon) return false;
	
	//Check If New Selected Weapon Is The Same As The Equipped Gun
	if (SelectedWeapon == EquippedGun)
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Equipping Same Gun"),this);
		return false;
	}
	
	//Hide The Last Current Equipped Weapon Actor
	if (EquippedGun)
	{
		EquippedGun->SetActorHiddenInGame(true);
		EquippedGun->SetActorEnableCollision(false);
	}
	
	//Enable Visibility and Collision On The Selected Weapon
	SelectedWeapon->SetActorHiddenInGame(false);
	SelectedWeapon->SetActorEnableCollision(true);
	
	//After Set The Weapon To The Current Equipped Gun
	EquippedGun = SelectedWeapon;
	
	OnEquipWeaponAnimation.Broadcast(SelectedWeapon->GetEquipAnimation());
	OnUpdateCurrentEquippedGun.Broadcast(EquippedGun->GetWeaponIcon());
	OnUpdateCurrentEquippedGunAmmo.Broadcast(EquippedGun->GetAmmoIcon(), EquippedGun->GetCurrentAmmo(), EquippedGun);
	return true;
}

void UInventoryComponent::CheckAddItemToHotbar(AWeaponBase* SpawnWeapon)
{
	//Check Every Array Index of Hotbar Weapon To See If There's a Null (Empty Slot) to Add Weapon That Spawn
	//If The Array Is Already Contains Something It Will Loop and Add The "Index" Number To +1 and Check That Array Index
	for (int32 Index = 0; Index < HotbarWeapons.Num(); Index++)
	{
		if (!HotbarWeapons[Index])
		{
			HotbarWeapons[Index] = SpawnWeapon;
			break;
		}
	}
}

void UInventoryComponent::MoveWeaponHotbar(const int32 DragSlotIndex, const int32 DropSlotIndex)
{
	//Check If Swap Slot Already Has Weapon In It
	if (!HotbarWeapons[DropSlotIndex])
	{
		//Swap The Drop Weapon Index Into The Drag Weapon Index
		HotbarWeapons.Swap(DragSlotIndex, DropSlotIndex);
	}
	else
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Already Item On This Hotbar"), this);
	}
}
#pragma endregion 

#pragma region Inventory
void UInventoryComponent::RefreshInventory()
{
	//Just To Note The FInventory Struct Has UItemDataAsset Automatically Sign When Interact With Actor (Can Check Each)
	//Another Note If For Loop is True It Will Break The For Loop
	//(Example: Index is 5 and InventoryItems.Num() Overall is 4 the Index is Higher Than The Inventory So True)
	
	//To Check If We Already Has The Item Inside Inventory Array
	bool bAlreadyHas = false;
	
	//For Each Loop Based On The Number Of Inventory Array
	for (int32 Index = 0; Index < InventoryItems.Num(); Index++)
	{
		//Set InsideInventoryData Struct Based On The Struct On The Given Index
		//Check If The Struct Data Asset Tag From Item That We Interact With Is The Same As The Inside Inventory Data Struct Tag
		if (FInventoryData& InsideInventoryData = InventoryItems[Index]; InventoryDataStruct.ItemDataAsset->ItemTag.MatchesTagExact(InsideInventoryData.ItemDataAsset->ItemTag))
		{
			//Add The Struct Item Quantity The Same Amount Of The Original Item Quantity In The Struct That Interact With
			InsideInventoryData.ItemQuantity += InventoryDataStruct.ItemDataAsset->ItemQuantity;
			
			//Broadcast To MasterInventory Just To Update The Quantity Rather Than Update The Whole THing With New Dispatcher
			OnUpdateNewQuantity.Broadcast(InsideInventoryData.ItemQuantity, Index);
			
			bAlreadyHas = true;
			break;
		}
	}
	
	//This Check Is For Item Data That First Time Got Added Into Inventory
	//If False That Mean Inside The Inventory There's No Item The Same As The Interact Item We Picked Up
	if (!bAlreadyHas)
	{
		// Add Interact Item Data Asset To Array For Other Use
		InventoryItems.Add(InventoryDataStruct);
		
		if (InventoryDataStruct.ItemDataAsset->ItemTag.MatchesTag(AcceptedUnlockDoorKey))
		{
			//Broadcast Update Key Unlock Door UI
			OnUpdateUnlockDoor.Broadcast(
				InventoryDataStruct.ItemDataAsset->ItemIcon, 
				InventoryDataStruct.ItemDataAsset->ItemTag,
				InventoryDataStruct.ItemDataAsset->ItemName);
		}
		
		//Broadcast The First Item Data Into Inventory
		OnUpdateInventory.Broadcast( 
				InventoryDataStruct.ItemDataAsset->ItemIcon,
				InventoryDataStruct.ItemDataAsset->ItemName,
				InventoryDataStruct.ItemQuantity);
	}
}

void UInventoryComponent::AddPickableToInventory_Implementation(AActor* PickableActor)
{
	if (!PickableActor) return;
	
	//Get FInventoryDataStruct& From The Interact Actor Base (Because Of & We Don't Want A copy That Why The Getter For This Is Different)
	//Insert Inventory Data Variable (The Struct) as The Second Argument To Save The Get Function Data Struct
	Execute_GetInventoryDataStruct(PickableActor, InventoryDataStruct);
	
	if (InventoryDataStruct.ItemDataAsset->ItemTag.MatchesAny(AcceptedInventoryTag))
	{
		//Called Refresh Inventory Functions
		RefreshInventory();
	}
}

bool UInventoryComponent::CalculatedNeedAmmo()
{
	if (AWeaponBase* WeaponRef = EquippedGun)
	{
		// Calculating How Much Ammo Does The Weapon Need
		const int32 MissingAmmo = WeaponRef->GetMaxAmmo() - WeaponRef->GetCurrentAmmo();
		
		for (int32 Index = 0; Index < InventoryItems.Num(); Index++)
		{
			//Checking Inside Inventory If It Has The Same Ammo Type Tag As Weapon Ref (AmmoType Tag Is Inside Weapon Base) 
			if (FInventoryData& CheckAmmoInventory = InventoryItems[Index]; WeaponRef->GetAmmoType().MatchesTag(CheckAmmoInventory.ItemDataAsset->ItemTag))
			{
				//When Match, Found Ammo Will Be Set As The Ammo Quantity Inside The Inventory
				if (int32& FoundAmmo = CheckAmmoInventory.ItemQuantity)
				{
					//Check If Found Ammo (Ammo Inside Inventory) Above 0 and Weapon Current and Max Ammo Is Not Same
					if (FoundAmmo > 0 &&  WeaponRef->GetCurrentAmmo() != WeaponRef->GetMaxAmmo())
					{
						// It Checks Between The Two Argument Which One Is The Smallest One and Saved That As "BulletToLoad" Value
						const int32 BulletToLoad = FMath::Min(MissingAmmo, FoundAmmo);
			
						// Then Remove The Bullet In Our Inventory Based On How Much Does "BulletToLoad" Take
						FoundAmmo -= BulletToLoad;
			
						// Also Reload To Current Weapon Magazine Based On How Much Does "BulletToLoad" Take
						WeaponRef->LoadAmmo(BulletToLoad);
						
						//Update The New Ammo Quantity Everytime Weapons is Reload
						OnUpdateNewQuantity.Broadcast(FoundAmmo, Index);
					
						WeaponRef->SetCurrentlyReloading(false);
						return true;
					}
				}
			}
		}
	}
	return false;
}
#pragma endregion

void UInventoryComponent::CorrectDoorKey_Implementation(const FGameplayTag CorrectDoorTag)
{
	for (int32 Index = 0; Index < InventoryItems.Num(); Index++)
	{
		const FInventoryData& CheckKeyInventory = InventoryItems[Index]; 
		
		if (CheckKeyInventory.ItemDataAsset->ItemTag.MatchesTag(CorrectDoorTag))
		{
			OnRemoveUnlockDoorKey.Broadcast(CheckKeyInventory.ItemDataAsset->ItemTag);
			OnRemoveItemInventory.Broadcast(Index);
			InventoryItems.RemoveAt(Index);
			OnCloseUnlockDoorUI.Broadcast();
			break;
		}
	}
}