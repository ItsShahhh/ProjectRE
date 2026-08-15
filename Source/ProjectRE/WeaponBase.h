// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableBase.h"
#include "EWeaponTypeBase.h"
#include "ItemDataAsset.h"
#include "WeaponBase.generated.h"

class AEmptyShell;
class USpringArmComponent;
class UCombatPhysicsMaterial;
class AProjectReCharacter;
class UAimingWeapon;
class UNiagaraSystem;
class UCameraComponent;
class UMasterHotbar;
class UAudioComponent;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdate, int32, NewAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckIsAiming, bool, bIsAiming);

UCLASS()
class PROJECTRE_API AWeaponBase : public APickableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> MuzzleVfxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon")
	TObjectPtr<USceneComponent> LeftHandSocket;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> EmptyShellComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon")
	TObjectPtr<UWidgetComponent> AmmoWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Weapon")
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Recoil")
	float WeaponRecoilForce = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	int32 MaxAmmo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Bullet")
	int32 CurrentAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	int32 BulletPellet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	float BulletSpread;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	int32 MaxRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	float FireRate;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Bullet")
	bool bCanShoot = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Bullet")
	TSubclassOf<AEmptyShell> EmptyShells;
	
	UPROPERTY(EditAnywhere, Category= "Bullet")
	float EmptyShellLaunch = 200.0f;
	
	UPROPERTY(EditAnywhere, Category= "Bullet")
	TSubclassOf<UCameraShakeBase> CameraShakeRecoil;
	
	UPROPERTY(EditAnywhere, Category= "VFX")
	TObjectPtr<UNiagaraSystem> MuzzleVFX;
	
	UPROPERTY(EditAnywhere, Category= "Bullet Hole")
	TObjectPtr<UMaterialInterface> BulletHoleMaterial;
	
	UPROPERTY(EditAnywhere, Category= "Bullet Hole")
	FVector DecalSize = FVector(10.0f, 10.0f, 10.0f);
	
	UPROPERTY(EditAnywhere, Category= "Audio")
	TObjectPtr<UAudioComponent> FireAudio;
	
	UPROPERTY(EditAnywhere, Category= "Audio")
	TObjectPtr<UAudioComponent> ReloadAudio;
	
	UPROPERTY(EditAnywhere, Category= "Audio")
	TObjectPtr<UAudioComponent> HolsterAudio;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Damage")
	int32 WeaponDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Damage")
	float RangeMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Damage")
	float RangeDivision = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Damage")
	float DistanceRangeDamage = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Damage")
	TObjectPtr<UNiagaraSystem> BloodSplatterVfx;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Animation")
	TObjectPtr<UAnimMontage> WeaponReloadAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Animation")
	TObjectPtr<UAnimMontage> EquipAnimation;
	
	UPROPERTY(EditAnywhere, Category= "Animation")
	FGameplayTag AmmoType;
	
	// Aiming Weapon
	UPROPERTY(VisibleAnywhere, Category= "Aiming")
	float MinAimingClamp = -45.0f;
	
	UPROPERTY(EditAnywhere, Category= "Aiming")
	float MaxAimingClamp = 45.0f;
	
	UPROPERTY(EditAnywhere, Category= "Aiming")
	float AimingSpeed = 20.0f;
	
	UPROPERTY(EditAnywhere, Category= "Aiming")
	float AimingWalkSpeed = 200.0f;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsFullyAim = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	TObjectPtr<UTexture2D> WeaponAmmoIcon;
	
	UPROPERTY(EditAnywhere, Category= "Socket")
	FName WeaponSocketName;
	
	UPROPERTY(BlueprintAssignable, Category= "Event")
	FOnCheckIsAiming OnCheckIsAiming;
	
	// Functions
	void BulletHit(FHitResult& HitResult, const FVector& ShootingStartLocation) const;

	void SpawnEmptyShells() const;
	
	void WeaponRecoil();
	
private:
	UPROPERTY()
	int32 NeededWeaponAmmo;
	
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY()
	bool bCurrentlyReloading = false;
	
	bool bDismembered = false;

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoUpdate OnAmmoUpdate;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsAimWeapon = false;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual bool FireWeapon(const FVector& CameraLocation, const FVector& CameraDirection);
	
	void StartAimingWeapon();
	
	void StopAimingWeapon();
	
	void LoadAmmo(int32 BulletToLoad);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CalledStartAiming();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CalledStopAiming();
	
	virtual void  Interactable_Implementation(AActor* InteractingActor) override;
	
	virtual void GetInventoryDataStruct_Implementation(FInventoryData& InventoryData) override;
	
	FORCEINLINE USceneComponent* GetLeftHandSocket() const { return LeftHandSocket; }
	
	FORCEINLINE USceneComponent* GetMuzzleComp() const { return MuzzleVfxComp; }
	
	FORCEINLINE float GetMinAimingClamp() const { return MinAimingClamp; }
	
	FORCEINLINE float GetMaxAimingClamp() const { return MaxAimingClamp; }
	
	FORCEINLINE float GetAimingWalkSpeed() const { return AimingWalkSpeed; }
	
	FORCEINLINE int32 GetMaxAmmo() const {return MaxAmmo; }
	
	FORCEINLINE int32 GetCurrentAmmo() const {return CurrentAmmo; }
	
	FORCEINLINE bool GetCurrentlyReloading() const {return bCurrentlyReloading; }
	
	FORCEINLINE void SetCurrentlyReloading(const bool NewCurrentlyReloading) {bCurrentlyReloading = NewCurrentlyReloading;} 
	
	FORCEINLINE UAnimMontage* GetReloadAnimation() const { return WeaponReloadAnimation; }
	
	FORCEINLINE UAnimMontage* GetEquipAnimation() const { return EquipAnimation; }
	
	FORCEINLINE TSubclassOf<UCameraShakeBase> GetCameraShakeClass() const { return CameraShakeRecoil; }
	
	FORCEINLINE bool GetCanShoot() const { return bCanShoot; }
	
	FORCEINLINE bool GetFullyAim() const { return bIsFullyAim; }
	
	FORCEINLINE EWeaponType GetWeaponType() const {return WeaponType;}
	
	FORCEINLINE void SetWeaponType(const EWeaponType NewWeaponType) { WeaponType = NewWeaponType; }
	
	FORCEINLINE FName GetWeaponSocketName() const { return WeaponSocketName; }
	
	FORCEINLINE UTexture2D* GetWeaponIcon() const { return ItemDataAsset
		->ItemIcon; }
	
	FORCEINLINE UTexture2D* GetAmmoIcon() const { return WeaponAmmoIcon; }
	
	FORCEINLINE FGameplayTag GetAmmoType() const { return AmmoType; }
	
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return StaticMesh; }
	
	FORCEINLINE UAudioComponent* GetHolsterAudio() const { return HolsterAudio; };
	
	FORCEINLINE UAudioComponent* GetReloadAudio() const { return ReloadAudio; };
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
