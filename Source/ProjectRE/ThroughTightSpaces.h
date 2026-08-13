// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "Components/ActorComponent.h"
#include "ThroughTightSpaces.generated.h"

class ATightSpaces;
class USpringArmComponent;
class AProjectReCharacter;
class UCharacterMovementComponent;
class UArrowComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTightSpaces);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTightSpaceForAnimation, const bool, bIsThroughTightSpaces);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRE_API UThroughTightSpaces : public UActorComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThroughTightSpaces();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bTroughTightSpaces = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateTightSpaceForAnimation OnUpdateTightSpaceForAnimation;
	
private:
	UPROPERTY()
	TObjectPtr<AActor> TightSpaceActor;
	
	UPROPERTY()
	TObjectPtr<ACharacter> OwningCharacter;
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComp;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY()
	TObjectPtr<USpringArmComponent> CameraComponent;
	
public:
	UPROPERTY(BlueprintReadOnly)
	FOnTightSpaces StartTightSpacesDispatcher;
	
	UPROPERTY(BlueprintReadOnly)
	FOnTightSpaces StopTightSpacesDispatcher;
	
	FORCEINLINE bool GetIsThroughTightSpaces() const { return bTroughTightSpaces; }
	
	FORCEINLINE AActor* GetTightSpaceActor() const { return TightSpaceActor; }
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void BeginEnvironmentInteraction_Implementation(AActor* InteractingActor, UArrowComponent* ArrowComponent) override;
	
	virtual void EndEnvironmentInteraction_Implementation() override;
	
	void TightSpacesMovement(float Forward) const;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
