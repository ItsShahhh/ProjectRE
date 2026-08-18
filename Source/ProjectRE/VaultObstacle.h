// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VaultObstacle.generated.h"

enum class ECharacterState : uint8;
class AProjectReCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVaultObstacle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMantleVault, const FVector&, VaultLocation, UAnimMontage*, VaultAnimation);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRE_API UVaultObstacle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVaultObstacle();

protected:
	UPROPERTY(EditAnywhere, Category= "Vault")
	float LineRange = 100.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FHitResult HitSphereVaultObstacle;
	
	UPROPERTY(EditAnywhere, Category= "Vault")
	float SphereUp = 100.0f;
	
	UPROPERTY(EditAnywhere, Category= "Vault")
	float MantleDistance = 50.0f;
	
	UPROPERTY(EditAnywhere, Category= "Vault")
	float VaultDistance = 210.0f;
	
	UPROPERTY(EditAnywhere, Category= "Vault")
	TObjectPtr<UAnimMontage> MantleAnimation;
	
	UPROPERTY(EditAnywhere, Category= "Vault")
	TObjectPtr<UAnimMontage> VaultAnimation;
	
	// Functions
	UFUNCTION(BlueprintCallable)
	void FinishMantleVault();
	bool CanVault(FVector VaultLocation) const;
	bool CanMantle() const;
	void CheckingVaultState() const;
	
private:
	bool bHitWaist = false;
	bool bVaulting = false;
	
	FHitResult HitObstacle;
	
	UPROPERTY()
	TObjectPtr<ACharacter> CharacterRef;

public:	
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FVector VaultingHandPlacement() const;
	void Mantle() const;
	void Vaulting() const ;
	void VaultObstacle();
	
	FOnVaultObstacle OnCheckingVaultState;
	FOnVaultObstacle OnFinishVaulting;
	FOnMantleVault OnMantleVault;
	FOnMantleVault OnVault;
};
