// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_ClimbingLadder UMETA(DisplayName = "Climbing Ladder"),
	ECS_TightSpace UMETA(DisplayName = "Through Tight Space"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_Aiming UMETA(DisplayName = "Aiming"),
	ECS_Sprint UMETA(DisplayName = "Sprint"),
	ECS_Falling UMETA(DisplayName = "Falling"),
	ECS_Interact UMETA(DisplayName = "Interact"),
	ECS_Mantle UMETA(DisplayName = "Mantle"),
	ECS_Vaulting UMETA(DisplayName = "Vault"),
};

class PROJECTRE_API FCharacterState
{
public:
	FCharacterState();
	~FCharacterState();
};
