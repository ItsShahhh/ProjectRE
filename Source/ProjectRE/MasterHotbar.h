// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterHotbar.generated.h"

class UHotbarData;
/**
 * 
 */
UCLASS()
class PROJECTRE_API UMasterHotbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearAndEmpty(UHotbarData* ItemData);
	
};
