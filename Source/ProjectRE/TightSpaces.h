// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentalInteractable.h"
#include "TightSpaces.generated.h"

class UThroughTightSpaces;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class PROJECTRE_API ATightSpaces : public AEnvironmentalInteractable
{
	GENERATED_BODY()
public:
	ATightSpaces();
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> EndTightSpaces;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComp;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterRef;
	
	UPROPERTY()
	TObjectPtr<UThroughTightSpaces> TightSpacesRef;
	
	UFUNCTION()
	void ReachEndOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) ;

public:
	virtual void Interactable_Implementation(AActor* InteractingActor) override;
};
