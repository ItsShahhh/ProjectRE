// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayUtils.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRE_API UGameplayUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static void PrintDebug(const float TimeToDisplay,const FColor TextColor,const FString& PrintText, const UObject* ThisObject);
};
