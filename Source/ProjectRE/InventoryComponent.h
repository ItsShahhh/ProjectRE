// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EWeaponTypeBase.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "InventoryDataStruct.h"
#include "PlayerStateInterface.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseUnlockDoorUI);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveUnlockDoorKey, FGameplayTag, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipWeaponAnimation, UAnimMontage*, EquipAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCurrentEquippedGun, UTexture2D*, CurrentGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveItemInventory, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHotbar, UTexture2D*, ItemIcon, FName, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveItemHotbar, int32, DragSlotIndex, int32, DropSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateNewQuantity, int32, NewQuantity, int32, CurrentItemIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateInventory, UTexture2D*, ItemIcon, FName, ItemName, int32, ItemQuantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateCurrentEquippedGunAmmo, UTexture2D*, CurrentWeaponAmmoIcon, int32, CurrentWeaponAmmo, AWeaponBase*, WeaponRef);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateUnlockDoor, UTexture2D*, KeyIcon, FGameplayTag, KeyTag,FName, KeyName);

struct FInventoryData;
class AAmmoBase;
class AWeaponBase;
class AProjectReCharacter;
class UClimbingLadder;
class UThroughTightSpaces;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRE_API UInventoryComponent : public UActorComponent, public IInteractableInterface, public IPlayerStateInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Weapon")
	TObjectPtr<AWeaponBase> EquippedGun;
	
	UPROPERTY(VisibleAnywhere,Category = "Inventory")
	TArray<FInventoryData>InventoryItems;
	
	UPROPERTY(EditDefaultsOnly,Category = "Inventory")
	TMap<EWeaponType, FName> HoldingWeaponSocket;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hotbar")
	TArray<AWeaponBase*> HotbarWeapons;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer AcceptedInventoryTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer AcceptedHotbarTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag AcceptedUnlockDoorKey;
	
	UPROPERTY(EditAnywhere, Category = "Hotbar")
	int32 NumberOfHotbar = 4;
	
	UPROPERTY(EditDefaultsOnly, Category = "Struct")
	FInventoryData InventoryDataStruct;

private:
	//Functions
	void CheckAddItemToHotbar(AWeaponBase* SpawnWeapon);
	void RefreshInventory();
	
public:
	
	UPROPERTY(BlueprintCallable,  Category = "Events")
	FOnCloseUnlockDoorUI OnCloseUnlockDoorUI;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateCurrentEquippedGun OnUpdateCurrentEquippedGun;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateCurrentEquippedGunAmmo OnUpdateCurrentEquippedGunAmmo;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateHotbar OnUpdateHotbar;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateInventory OnUpdateInventory;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateUnlockDoor OnUpdateUnlockDoor;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnUpdateNewQuantity OnUpdateNewQuantity;
	
	UPROPERTY(BlueprintCallable, Category = "Events")
	FOnMoveItemHotbar OnMoveItemHotbar;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRemoveUnlockDoorKey OnRemoveUnlockDoorKey;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnRemoveItemInventory OnRemoveItemInventory;
	
	UPROPERTY(BlueprintReadOnly, Category = "Events")
	FOnEquipWeaponAnimation OnEquipWeaponAnimation;
	
	FORCEINLINE AWeaponBase* GetEquippedGun() const {return EquippedGun;}
	
	//Functions
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Unlock Door
	virtual void CorrectDoorKey_Implementation(FGameplayTag CorrectDoorTag) override;
	
	//Inventory
	bool CalculatedNeedAmmo();
	bool CheckEquipWeapon(int32 WeaponSlot);
	virtual void AddWeaponToInventory_Implementation(AWeaponBase* PickedUpWeapon, AActor* PickableActor) override;
	virtual void AddPickableToInventory_Implementation(AActor* PickableActor) override;
	
	//Hotbar
	UFUNCTION()
	void MoveWeaponHotbar(int32 DragSlotIndex, int32 DropSlotIndex);
};
