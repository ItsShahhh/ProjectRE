// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Handgun     UMETA(DisplayName = "Handgun"),
	Shotgun     UMETA(DisplayName = "Shotgun"),
	Rifle     UMETA(DisplayName = "Rifle"),
	Grenade   UMETA(DisplayName = "Grenade")
};

class PROJECTRE_API EWeaponTypeBase
{
public:
	EWeaponTypeBase();
	~EWeaponTypeBase();
};
