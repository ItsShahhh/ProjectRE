// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableTrace.generated.h"

class APickableBase;
class AInteractableItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRE_API UInteractableTrace : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableTrace();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "LineTrace")
	float LineTraceRange = 500.0f;

private:
	FHitResult HitResult;
	
	bool IsFacingInteractable(const AActor* ItemActor) const;

public:
	// Functions
	void TraceTimerScanner(const FVector& StartTraceLocation, const FVector& ForwardTraceLocation);
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
