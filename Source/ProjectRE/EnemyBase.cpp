// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIController.h"
#include "GameplayTagContainer.h"
#include "Health.h"
#include "QuestSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "DamageableInterface.h"
#include "GameplayUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComp = CreateDefaultSubobject<UHealth>("HealthComp");
	
	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>("Dissolve Timeline");
	
	PhysicalAnimationComp = CreateDefaultSubobject<UPhysicalAnimationComponent>("PhysicalAnimationComp");
	
	FootstepAudioComp = CreateDefaultSubobject<UAudioComponent>("FootstepSoundsComponent");
	FootstepAudioComp->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	AIComp = Cast<AAIController>(GetController());
	
	if (AIComp)
	{
		if (UBlackboardComponent* BlackboardComp = AIComp->GetBlackboardComponent())
		{
			//Inside BlackBoard For SplinePatch Actor We Set To This
			BlackboardComp->SetValueAsObject(FName("SplinePath"), AssignSpline);
		}
	}
	
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>();
	}
	
	PhysicalAnimationComp->SetSkeletalMeshComponent(GetMesh());
	PhysicalAnimationComp->ApplyPhysicalAnimationProfileBelow(FName("spine_01"), FName("HitReaction"), false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	// Stumble Physical Animation Settings
	FPhysicalAnimationData SmoothSettings;
	SmoothSettings.OrientationStrength = 500.0f;
	SmoothSettings.PositionStrength = 500.0f;
	SmoothSettings.AngularVelocityStrength = 100.0f; 
	SmoothSettings.VelocityStrength = 100.0f; 
	SmoothSettings.bIsLocalSimulation = false;
	PhysicalAnimationComp->ApplyPhysicalAnimationSettingsBelow("spine_01", SmoothSettings, false);
}

// Called every frame
void AEnemyBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::CalculatedAttack()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this); // Ignore self

	TArray<AActor*> OutActors;
	const FVector EnemyLocation = GetActorLocation();
	const FVector ForwardVector = GetActorForwardVector();
	const FVector AttackLocation = EnemyLocation + ForwardVector * AttackRange;
	
	const bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AttackLocation, 32,ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
	DrawDebugSphere(GetWorld(), AttackLocation, 32, 12, FColor::Red, false,1, 0, 1);
	
	if (!bHit) return;
	
	for (AActor* HitActor : OutActors)
	{
		if (!HitActor) continue;
		
		if (HitActor->Implements<UDamageableInterface>())
		{
			const FGameplayTag HitActorTag = Execute_GetAffiliationTag(HitActor);
			
			//When Enemy Attack It Also Check If Any Overlap Actor Has The Same Tag, If True That Mean Their Hitting Each Other
			if (HitActorTag.MatchesTag(AffiliationTag)) continue;
			
			// Check If Hit Actor Implement Damageable Interface
			Execute_HitCameraShake(HitActor);
			Execute_OnReceiveDamage(HitActor, AttackDamage, DummyHitResult);
			break;
		}
	}
}

void AEnemyBase::EnemyAttack_Implementation()
{
	bIsAttacking = true;
	if (!AttackAnimation) return;
	PlayAnimMontage(AttackAnimation);
}

void AEnemyBase::FinishAttackAnimation()
{
	bIsAttacking = false;
	AttackFinishedSignature.Broadcast();
}

void AEnemyBase::StartDissolve()
{
	if (!DissolveCurve) return;
	if (!DissolveMaterial) return;
	
	// Getting Enemy Mesh Materials Index
	const int32 MaterialIndex = GetMesh()->GetNumMaterials();
	
	for (int32 i = 0; i < MaterialIndex; i++)
	{
		// Overwrite The Current Materials and Change It To The Dissolve Material For Each Of The Enemy Materials
		if (UMaterialInstanceDynamic* DyMaterial = GetMesh()->CreateDynamicMaterialInstance(i, DissolveMaterial))
		{
			DynamicDissolveMaterials.Add(DyMaterial);
		}
	}
	
	FOnTimelineFloat TimelineProgress;
	// Attach TimeLine Progress To Update Dissolve Material Function Below
	TimelineProgress.BindUFunction(this, "UpdateDissolveMaterial");
	// Adding Float Value Using The Curve Float, So When Using "DissolveValue" It Knows Where To Take The Float Value From
	DissolveTimeline->AddInterpFloat(DissolveCurve, TimelineProgress);
	DissolveTimeline->PlayFromStart();
	
	// Set Timer For When The Dissolve Is Fully Finish It Destroy The Enemy Actor
	const float DissolveLenght = DissolveTimeline->GetTimelineLength();
	
	FTimerHandle DestroyTimer;
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, [this]
	{
		Destroy();
	},
	DissolveLenght,
	false
	);
}

void AEnemyBase::UpdateDissolveMaterial(const float DissolveValue)
{
	for (UMaterialInstanceDynamic* DynMaterial : DynamicDissolveMaterials)
	{
		if (DynMaterial)
		{
			DynMaterial->SetScalarParameterValue(TEXT("Disolve"), DissolveValue);
		}
	}
}

void AEnemyBase::BodyDismemberment_Implementation(FHitResult& HitResult, const float WeaponRealDamage)
{
	// Get Hit Result Bone  Name
	const FName BodyPartName = HitResult.BoneName;
	
	// Check Does The TMap Has The Bone Name
	if (!LimbsHealth.Contains(BodyPartName)) return;
	
	// If True Deducted The Float On The TMap Based On The Bone Name
	LimbsHealth[BodyPartName] -= WeaponRealDamage;
	
	// If Body Part Health Is Less Than 0 Will The Function Continue To Run
	if (LimbsHealth[BodyPartName] > 0) return;
	
	// Using Hit Result Bone Name We Change It To String and Then Added an "_Dismember" Text At The End Of Hit Result Bone Name Because The Dismember Spawn Only On Socket That Name (lowerarm_l_Dismember)
	const FString CheckBodyPartSocket = BodyPartName.ToString() + TEXT("_Dismember");
	// After That We Changed The FString To FName
	const FName BodyPartSocketName = FName(CheckBodyPartSocket);
	
	// Safety Check In Case Forgot To Put Socket On The Bone
	if (!GetMesh()->DoesSocketExist(BodyPartSocketName)) return;
	
	// If True We Get The Location and Rotation Of That Socket Bone
	const FVector BodyPartLocation = GetMesh()->GetSocketLocation(BodyPartSocketName);
	const FRotator BodyPartRotation =  GetMesh()->GetSocketRotation(BodyPartSocketName);
	UE_LOG(LogTemp, Log, TEXT("BodyPart = %s"), *BodyPartName.ToString());
	
	// Spawn Fake Hand On The Socket Location and Rotation
	GetMesh()->HideBoneByName(BodyPartName, PBO_Term);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Using TMap As Class Actor To Spawn The Arm
	AActor* LimbsSpawn = GetWorld()->SpawnActor<AActor>(SpawnLimbs[BodyPartName], BodyPartLocation, BodyPartRotation, SpawnParams);
	if (!IsValid(LimbsSpawn)) return;
	
	// After Certain Amount Of Time This Spawn limbs Will Get Destroyed
	LimbsSpawn->SetLifeSpan(3.0f);
	
	// Change Attack Animation According To The Limbs That Get Destroyed
	AttackAnimation = SevereLimbsAnimation[BodyPartName];
	
	// Change To Another Attack Animation If Both Limbs is Detached
	if (LimbsHealth["lowerarm_l"] < 0 && LimbsHealth["lowerarm_r"] < 0)
	{
		if (NoLimbsAnimation)
		{
			AttackAnimation = NoLimbsAnimation;
		}
	}
}

void AEnemyBase::StumbleHit_Implementation(FHitResult& HitResult)
{
	// Don't Called Stumble If Enemy Is Started Attacking
	if (bIsAttacking) return;
	
	//Get Bone Name From Hit Result
	const FName& HitBone = HitResult.BoneName;
	
	//Get The Hit Bone Parent
	const FName CheckBone = GetMesh()->GetParentBone(HitBone);
	
	//Check If The Hit Bone Parent or Hit Bone Itself Is "thigh_l" or "thigh_r" Then Don't Simulate Physics
	if (CheckBone == "thigh_l" || HitBone  == "thigh_l") return;
	if (CheckBone == "thigh_r" || HitBone == "thigh_r") return;
	
	// GetSafeNormal() handles the distance and normalization safely in one line!
	const FVector BulletDirection = -HitResult.ImpactNormal;
	const FVector ImpulseDirection = BulletDirection * StumbleForce;
	
	GetMesh()->SetAllBodiesBelowSimulatePhysics(HitBone, true);
	GetMesh()->AddImpulse(ImpulseDirection, HitBone, false);
	
	// This Function Is Called Inside Enemy Blueprint
	StartRecoveryBlueprint(HitBone);
}

void AEnemyBase::OnReceiveDamage_Implementation(const float ReceiveDamage, const FHitResult& HitResult)
{
	HealthComp = this->FindComponentByClass<UHealth>();
	if (!IsValid(HealthComp)) return;
	HealthComp->ReceiveDamage(ReceiveDamage);
	
	if (!BloodSplatterVfx) return;

	// Using Actor Forward Actor Then Make It Negative So It Became Backward
	const FVector BloodSplatterDirection = -GetActorForwardVector();
	const FRotator BloodSplatterRotation = BloodSplatterDirection.Rotation();
	
	// Spawn In Blood Splatter VFX
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatterVfx, HitResult.ImpactPoint, BloodSplatterRotation, FVector(1), true);
}

void AEnemyBase::ReactToExplosion_Implementation(FVector& LaunchForces)
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(LaunchForces, NAME_None, true);
}

void AEnemyBase::Died_Implementation()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetController()->StopMovement();
	StartDissolve();
	
	if (IsValid(QuestSubsystem) && EliminateQuestTag.IsValid())
	{
		QuestSubsystem->ReportGameplayEvent(EliminateQuestTag);
	}
	
	if (!AIComp) return;
	AIComp->UnPossess();
}

void AEnemyBase::FootstepSound()
{
	//Footstep Line Trace Location
	const FVector EnemyStep = GetActorLocation() + (-GetActorUpVector() * 100);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	
	//The Hit Result Could Be Hitting The Actor Itself So It Will Get The Actor Material Instead Of Ground
	QueryParams.AddIgnoredActor(this);
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), EnemyStep, ECC_Visibility, QueryParams);
	
	if (bHit)
	{
		//Check Physical Material Surface Type (Can Be Change Inside Physics Material Setting)
		const EPhysicalSurface GroundMaterial = HitResult.PhysMaterial->SurfaceType;
		if (!GroundMaterial) return;
		
		// Check If The TMap Contains The Type Surface That We Give
		if (!FootstepSoundsType.Contains(GroundMaterial))
		{
			UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Doesn't Contain This Type Of Sound Or Material Inside TMap"), this);
			return;
		}
		// Set The SoundBaseOnSurface Based On The TMap Using Surface Type
		USoundBase* SoundBaseOnSurface = FootstepSoundsType[GroundMaterial];
		
		if (!IsValid(FootstepAudioComp)) return;
		FootstepAudioComp->SetSound(SoundBaseOnSurface);
		FootstepAudioComp->Play();
	}
}

FGenericTeamId AEnemyBase::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}

FOnAttackFinishedSignature* AEnemyBase::GetAttackFinishedDelegate()
{
	return &AttackFinishedSignature;
}

FGameplayTag AEnemyBase::GetAffiliationTag_Implementation() const
{
	return AffiliationTag;
}
