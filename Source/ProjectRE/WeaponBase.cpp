// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "CombatPhysicsMaterial.h"
#include "DamageableInterface.h"
#include "EmptyShell.h"
#include "EnemyInterface.h"
#include "GameplayUtils.h"
#include "Health.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("RootComponent ");
	SetRootComponent(SceneComp);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComp); 
	
	MuzzleVfxComp = CreateDefaultSubobject<USceneComponent>("MuzzleVFXComp");
	MuzzleVfxComp->SetupAttachment(StaticMesh);
	
	FireAudio = CreateDefaultSubobject<UAudioComponent>("Fire Audio Component");
	FireAudio->SetupAttachment(SceneComp);
	
	ReloadAudio = CreateDefaultSubobject<UAudioComponent>("Reload Audio Component");
	ReloadAudio->SetupAttachment(SceneComp);
	
	HolsterAudio = CreateDefaultSubobject<UAudioComponent>("Holster Audio Component");
	HolsterAudio->SetupAttachment(SceneComp);
	
	LeftHandSocket = CreateDefaultSubobject<USceneComponent>("Left Hand Socket Component");
	LeftHandSocket->SetupAttachment(StaticMesh);
	
	EmptyShellComp = CreateDefaultSubobject<USceneComponent>("Empty Shell Component");
	EmptyShellComp->SetupAttachment(StaticMesh);
	
	AmmoWidget = CreateDefaultSubobject<UWidgetComponent>("Ammo Widget");
	AmmoWidget->SetupAttachment(SceneComp);
	
	HolsterAudio->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
	
	CharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (AmmoWidget)
	{
		AmmoWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AmmoWidget->SetVisibility(false);
	}
}

// Called every frame
void AWeaponBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (!ItemDataAsset) return;
	
	//Save Inventory Data Struct Item So Don't Have To Set It Manually
	ItemStructs.ItemDataAsset = ItemDataAsset;
}

bool AWeaponBase::FireWeapon(const FVector& CameraLocation, const FVector& CameraDirection)
{
	// Early Checks For Shooting Weapons
	if (!bIsFullyAim) return false;
	if (!bCanShoot) return false;
	if (CurrentAmmo == 0) return false;

	const float SpreadInRadians = FMath::DegreesToRadians(BulletSpread);
	FCollisionQueryParams TraceParams;
	TraceParams.bReturnPhysicalMaterial = true;
	
	// Check if "i" is More Than Bullet Pellet, If True Line Trace Will be Shot According To The Bullet Pellet Number
	for (int i = 0; i < BulletPellet; i++)
	{
		FHitResult HitResult;
		FVector RandomTraceDirection = FMath::VRandCone(CameraDirection , SpreadInRadians);
		FVector EndLocation = CameraLocation + (RandomTraceDirection * MaxRange);
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, EndLocation, ECollisionChannel::ECC_Visibility, TraceParams);
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleMaterial, DecalSize, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), 5.0f);
		CurrentAmmo --;
		
		// Delegate For Updating Ammo UI To Check How Much Ammo Is Currently
		OnAmmoUpdate.Broadcast(CurrentAmmo);
		
		if (bHit)
		{
			// Call Calculate Damage Functions On Every LineTrace Hit
			BulletHit(HitResult, CameraLocation);
		}
	}
	// Stop Weapon From Firing Until The Timer Finish
	bCanShoot = false;
	FTimerHandle FireRateHandle;
	GetWorld()->GetTimerManager().SetTimer(FireRateHandle, [this]
	{
		bCanShoot = true;
	},
	FireRate,
	false
	);
		
	// Spawn an MuzzleVFX to MuzzleSceneComp From Each Weapon Location
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleVFX, MuzzleVfxComp->GetComponentLocation(), MuzzleVfxComp->GetComponentRotation(), FVector(1), true);
	
	//Spawn Empty Shell Out Of Gun
	SpawnEmptyShells();
	
	if (FireAudio)
	{
		// Play Weapon Shot Sound
		FireAudio->Play();
	}
	
	//When Shoot It Will Make a Noise That Alert Enemy
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, CharacterRef, 0.0f);
	
	// Change Dismember Boolean From True To False So Can Still Dismember Other Arm
	bDismembered = false;
	 
	// Return True When Weapon Fired
	return true;
}

void AWeaponBase::BulletHit(FHitResult& HitResult, const FVector& ShootingStartLocation) const
{
	// Get The Hit Result Actor
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;
	
	//Check If Other Actor Has Damageable Interface (Mostly This Is For Environment Actor Like Barrel)
	if (HitActor->Implements<UDamageableInterface>())
	{
		IDamageableInterface::Execute_OnReceiveImpact(HitActor);
		return;
	}
	
	UPhysicalMaterial* HitMaterial = HitResult.PhysMaterial.Get();
	const UCombatPhysicsMaterial* RealHitMaterial = Cast<UCombatPhysicsMaterial>(HitMaterial);
	if (!HitMaterial || !RealHitMaterial)
	{
		UGameplayUtils::PrintDebug(1.0f, FColor::Red, TEXT("You Forgot To Put Physic Material Inside This Enemy Physics Asset"), this);
		return;
	}
	
	// Calculate Damage With Multiplier and Division Inside The Physics Material and Based On Body Part
	const float DamageMultiplier = RealHitMaterial->GetDamageMultiplier();
	const float DamageDivider = RealHitMaterial->GetDamageDivider();
	float BodyPartDamage = WeaponDamage * DamageMultiplier / DamageDivider;
	
	// Checking Distance Between Player and Enemy when Bullet Hit An Enemy To Increase Or Reduce Damage Based On Distance
	const float CheckDistance = FVector::Dist(HitActor->GetActorLocation(), ShootingStartLocation);
	if (CheckDistance <=  DistanceRangeDamage)
	{
		BodyPartDamage *= RangeMultiplier;
	}
	else if (CheckDistance >=  DistanceRangeDamage)
	{
		BodyPartDamage /= RangeDivision;
	}
	// Real Damage After Calculated Body Part Damage and Distance Range Damage
	const float RealDamage = BodyPartDamage;
	
	// Check If Hit Actor Has Health Component
	if (UHealth* HealthComp = HitActor->FindComponentByClass<UHealth>())
	{
		//Call On Receive Damage On Damageable Interface
		//Set It To HealthComp Because The Interface Is Inside Health Component
		IDamageableInterface::Execute_OnReceiveDamage(HealthComp, RealDamage);
	}
	
	// Called Body Dismemberment Function
	if (HitActor->Implements<UEnemyInterface>())
	{
		//Called On Enemy Interface For Body Dismemberment and Stumble Hit (Pushed When Getting Hit)
		IEnemyInterface::Execute_BodyDismemberment(HitActor, HitResult, RealDamage);
		IEnemyInterface::Execute_StumbleHit(HitActor, HitResult);
	}
	
	// Using Camera Rotation as Blood Splatter Rotation (So The Blood Splatter VFX Always Spawn and Splatter Behind Enemy)
	const FRotator BloodSplatterRotation = CharacterRef->GetActorForwardVector().Rotation();
	
	// Spawn In Blood Splatter VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterVfx, HitResult.ImpactPoint, BloodSplatterRotation, FVector(1), true);
}

void AWeaponBase::SpawnEmptyShells() const
{
	if (!EmptyShells) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (AEmptyShell* SpawnEmptyShells = GetWorld()->SpawnActor<AEmptyShell>(EmptyShells, EmptyShellComp->GetComponentLocation(),EmptyShellComp->GetComponentRotation() , SpawnParams))
	{
		const FVector LaunchDirection =  -EmptyShellComp->GetRightVector();
		const FVector ImpulseEmptyShell = LaunchDirection * EmptyShellLaunch;
		
		SpawnEmptyShells->GetStaticMesh()->AddImpulse(ImpulseEmptyShell, NAME_None, true);
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [SpawnEmptyShells]()
		{
			if (IsValid(SpawnEmptyShells))
			{
				SpawnEmptyShells->Destroy();
			}
		}, 
		2.0f, 
		false 
		);
	}
}

void AWeaponBase::LoadAmmo(const int32 BulletToLoad)
{
	// If Available Using That Specific Ammo Type and Reload It To Equipped Weapon
	CurrentAmmo += BulletToLoad;
	bCurrentlyReloading = false;
	
	// Delegate For Updating Ammo UI To Check How Much Ammo Is Currently
	OnAmmoUpdate.Broadcast(CurrentAmmo);
}

void AWeaponBase:: Interactable_Implementation(AActor* InteractingActor)
{
	if (!IsValid(InteractingActor)) return;
	
	UInventoryComponent* InventoryComponentComp = InteractingActor->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(InventoryComponentComp)) return;
	
	//Check If The Interacting Actor Has Interactable Component Attach
	//If True Add Interact Weapon To Inventory
	Execute_AddWeaponToInventory(InventoryComponentComp, this, this);
	
	Execute_PickableItemIK(InteractingActor, GetActorLocation());
	Destroy();
}

void AWeaponBase::GetInventoryDataStruct_Implementation(FInventoryData& InventoryData)
{
	InventoryData = ItemStructs ;
}

void AWeaponBase::StartAimingWeapon()
{
	bIsAimWeapon = true;
	AmmoWidget->SetVisibility(true);
	
	//Update Check Aiming Weapon Variables For Player Animation
	OnCheckIsAiming.Broadcast(bIsAimWeapon);
}

void AWeaponBase::StopAimingWeapon()
{
	bIsAimWeapon = false;
	AmmoWidget->SetVisibility(false);
	
	//Update Check Aiming Weapon Variables For Player Animation
	OnCheckIsAiming.Broadcast(bIsAimWeapon);
}