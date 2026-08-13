// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiedInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "EnemyBase.generated.h"

class UQuestSubsystem;
class UCombatPhysicsMaterial;
class AAIController;
class UHealth;
class USplineComponent;
class UPhysicalAnimationComponent;

UCLASS()
class PROJECTRE_API AEnemyBase : public ACharacter, public IDiedInterface, public IGenericTeamAgentInterface, public IEnemyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	UPROPERTY(VisibleAnywhere)
	UPhysicalAnimationComponent* PhysicalAnimationComp;
	
	UPROPERTY(VisibleAnywhere)
	UHealth* HealthComp;
	
	// Dissolve Material
	UPROPERTY(EditDefaultsOnly, Category= "Dissolve")
	UCurveFloat* DissolveCurve;
	
	UPROPERTY(EditDefaultsOnly, Category= "Dissolve")
	UMaterialInterface* DissolveMaterial;
	
	UPROPERTY(VisibleAnywhere, Category= "Dissolve")
	UTimelineComponent* DissolveTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category= "Limbs")
	UAnimMontage* NoLimbsAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category= "Limbs")
	TMap<FName, UAnimMontage*> SevereLimbsAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category= "Limbs")
	TMap<FName, float> LimbsHealth;
	
	UPROPERTY(EditDefaultsOnly, Category= "Limbs")
	TMap<FName, TSubclassOf<AActor>> SpawnLimbs;
	
	// Attack Functions
	UPROPERTY(EditAnywhere, Category= "Attack")
	float StumbleForce = 2000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 AttackRange = 20;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage = 20;
	
	UPROPERTY(EditDefaultsOnly, Category= "Attack")
	UAnimMontage* AttackAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
	AActor* AssignSpline;
	
	UPROPERTY(EditDefaultsOnly, Category= "Quest")
	FGameplayTag EliminateQuestTag;
	
private:
	//Variables
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicDissolveMaterials;
	
	UPROPERTY()
	UQuestSubsystem* QuestSubsystem;
	
	UPROPERTY()
	AAIController* AIComp;
	
	FPhysicalAnimationData ResetSettings;
	FOnAttackFinishedSignature AttackFinishedSignature;
	
	bool bIsStumble = false;
	bool bIsAttacking = false;
	
	//Functions
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();
	
public:
	//Functions
	UFUNCTION(BlueprintCallable)
	void CalculatedAttack();
	
	UFUNCTION(BlueprintCallable)
	void FinishAttackAnimation();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartRecoveryBlueprint(FName RecoverBone);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Function To Give Team ID For Detection For Affiliation in AI Perception
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnAttackFinishedSignature* GetAttackFinishedDelegate() override;
	virtual void Died_Implementation() override;
	virtual void EnemyAttack_Implementation() override;
	virtual void BodyDismemberment_Implementation(FHitResult& HitResult, float WeaponRealDamage) override;
	virtual void StumbleHit_Implementation(FHitResult& HitResult) override;
	virtual void ReactToExplosion_Implementation(FVector& LaunchForces) override;
	
	//Getter Variables
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }
	FORCEINLINE UHealth* GetHealthComp() const { return HealthComp; }
};
