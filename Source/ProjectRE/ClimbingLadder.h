// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "Components/ActorComponent.h"
#include "ClimbingLadder.generated.h"

class AProjectReCharacter;
class UArrowComponent;
class UCharacterMovementComponent;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachTopClimbing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateClimbingForAnimation, const bool, bIsClimbing);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRE_API UClimbingLadder : public UActorComponent, public IInteractableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbingLadder();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* ClimbingLadderMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ClimbingLadderAudio;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsClimbing = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateClimbingForAnimation OnUpdateClimbingForAnimation;
	
private:
	UPROPERTY()
	ACharacter* OwningCharacter;
	
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	
	UPROPERTY()
	AActor* CurrentLadder;
	
public:
	// Event Dispatcher
	FOnReachTopClimbing OnReachTopClimbing;
	FOnReachTopClimbing OnStartClimbing;
	FOnReachTopClimbing OnStopClimbing;
	FOnReachTopClimbing OnFinishedClimbing;
	
	FORCEINLINE bool GetIsClimbing() const { return bIsClimbing; }
	
	// Functions
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//Called Every Tick
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void BeginEnvironmentInteraction_Implementation(AActor* InteractingActor, UArrowComponent* ArrowComponent) override;
	
	virtual void EndEnvironmentInteraction_Implementation() override;
	
	UFUNCTION(BlueprintCallable)
	void FinishedTopClimbing();
	
	UFUNCTION(BlueprintCallable)
	void PlayClimbingLadderAudio();
	
	void ClimbingMovement(const float Forward) const;
	
	void ReachedTopClimbing();
};
