// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncher.h"

#include "EnemyBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DamageableInterface.h"
#include "Health.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStaticsTypes.h"


void AGrenadeLauncher::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// On Grenade Launcher Timeline When Timeline Finished Then You Can See The Projectile Path
	if (bIsFullyAim)
	{
		ProjectilePath();
	}
}

bool AGrenadeLauncher::FireWeapon(const FVector& CameraLocation, const FVector& CameraDirection)
{
	// On Grenade Launcher Timeline When Timeline Finished Then Shoot Weapon
	if (!bIsFullyAim) return false;
	if (!bCanShoot) return false;
	if (CurrentAmmo == 0) return false;
	
	bCanShoot = false;
	FTimerHandle FireRateHandle;
	GetWorld()->GetTimerManager().SetTimer(FireRateHandle, [this]
	{
		bCanShoot = true;
	},
	FireRate,
	false
	);
	
	CurrentAmmo--;
	OnAmmoUpdate.Broadcast(CurrentAmmo);
	
	const FVector StartLocation = GetMuzzleComp()->GetComponentLocation();
	const FRotator StartRotation =  GetMuzzleComp()->GetComponentRotation();
	const FVector LaunchDirection =  GetMuzzleComp()->GetForwardVector();
	const FVector FinalImpulse = LaunchDirection * GLBulletForce;
	
	NewGLBullet = GetWorld()->SpawnActor<AActor>(GLBullet, StartLocation, StartRotation);
	
	if (NewGLBullet)
	{
		if (UStaticMeshComponent* GLMesh = NewGLBullet->FindComponentByClass<UStaticMeshComponent>())
		{
			GLMesh->SetSimulatePhysics(true);
			GLMesh->AddImpulse(FinalImpulse, NAME_None, true);
			GLMesh->OnComponentHit.AddDynamic(this, &AGrenadeLauncher::OnComponentHit);
		}
	}
	if (FireAudio)
	{
		FireAudio->Play();
	}
	return true;
}

void AGrenadeLauncher::ProjectilePath()
{
	const FVector StartLocation = GetMuzzleComp()->GetComponentLocation(); 
	const FVector LaunchDirection =  GetMuzzleComp()->GetForwardVector();

	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = StartLocation;
	PredictParams.LaunchVelocity = LaunchDirection * ProjectileTraceSpeed;
	PredictParams.ProjectileRadius = 10.0f; 
    
	PredictParams.bTraceWithCollision = true; 
	PredictParams.TraceChannel = ECC_Visibility; 
	PredictParams.MaxSimTime = 2.0f; 
	PredictParams.ActorsToIgnore.Add(this); 
	PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame; 
			
	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
}

void AGrenadeLauncher::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FVector ExplodeLocation = Hit.ImpactPoint;
	const FCollisionShape SphereShape = FCollisionShape::MakeSphere(ExplosionRadius);

	if (GetWorld()->SweepMultiByChannel(ExplodeHitResults, ExplodeLocation, ExplodeLocation, FQuat::Identity, ECC_Pawn, SphereShape))
	{
		ExplosionForce(ExplodeLocation);
		// If Bullet Hit Any Static Thing It Will Spawn An VFX and Destroy The Bullet
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GLExplosionVfx, ExplodeLocation, ExplodeLocation.Rotation(), FVector(1), true);
		NewGLBullet->Destroy();
		
		if (!ExplodeSound) return;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound, ExplodeLocation);
	}
}

void AGrenadeLauncher::ExplosionForce(FVector const& ExplodeLocation) const
{
	for (const FHitResult& CaughtActor : ExplodeHitResults)
	{
		//Continue Mean We Skip The Current Loop For The Actor That Null and Continue For The Next Actor
		AActor* HitActor = CaughtActor.GetActor();
		if (!HitActor) continue;
		
		//Check If Other Actor Has Damageable Interface (Mostly This Is For Environment Actor Like Barrel)
		if (HitActor->Implements<UDamageableInterface>())
		{
			IDamageableInterface::Execute_OnReceiveImpact(HitActor);
			continue;
		}
		
		//Check If Hit Actor Has Health Component
		if (UHealth* HealthComp = HitActor->FindComponentByClass<UHealth>())
		{
			IDamageableInterface::Execute_OnReceiveDamage(HealthComp, ExplosiveDamage);
		}
		
		if (HitActor->Implements<UEnemyInterface>())
		{
			const FVector EnemyLocation = HitActor->GetActorLocation();
			// Subtracting From Enemy Location To Explode Location To get The Direction For Impulse Launch
			FVector ForceDirection = EnemyLocation - ExplodeLocation;
			// Normalize The Calculation To Only 1,1,1 Because We Only Want To Know The Direction
			ForceDirection.Normalize();
			FVector LaunchForces = ForceDirection * ExplosionLaunch;
		
			IEnemyInterface::Execute_ReactToExplosion(HitActor, LaunchForces);
		}
	}
}