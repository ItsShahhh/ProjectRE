// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentalInteractable.h"
#include "Ladder.generated.h"

class UClimbingLadder;
/**
 * 
 */
UCLASS()
class PROJECTRE_API ALadder : public AEnvironmentalInteractable
{
	GENERATED_BODY()
public:
	ALadder();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* TopLadderCollision;

private:
	UFUNCTION()
	void ReachTopLadderOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY()
	ACharacter* CharacterRef;
	
	UPROPERTY()
	UClimbingLadder* ClimbingLadderRef;
	
public:
	virtual void BeginPlay() override;
	virtual void Interactable_Implementation(AActor* InteractingActor) override;
};
