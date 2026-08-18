// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "GrenadeLauncher.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class PROJECTRE_API AGrenadeLauncher : public AWeaponBase
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> GLBullet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FHitResult>ExplodeHitResults;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> GLExplosionVfx;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> ExplodeSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosiveDamage = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileTraceSpeed = 1500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionLaunch = 5000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GLFireRate = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GLBulletForce = 1500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ExplosionRadius = 512.0f;

private:
	
	FHitResult DummyHitResult;
	
	UPROPERTY()
	TObjectPtr<AActor> NewGLBullet;
	
	void ExplosionForce(const FVector& ExplodeLocation) const;
	void ProjectilePath();
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual bool FireWeapon(const FVector& CameraLocation, const FVector& CameraDirection) override;
	
};
