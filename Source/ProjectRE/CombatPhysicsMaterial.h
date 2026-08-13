// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CombatPhysicsMaterial.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRE_API UCombatPhysicsMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageDivider = 1.0f;

public:
	
	FORCEINLINE float GetDamageMultiplier() const { return DamageMultiplier; }
	
	FORCEINLINE float GetDamageDivider() const { return DamageDivider; }
	
};
