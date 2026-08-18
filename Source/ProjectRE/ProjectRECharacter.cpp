// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRECharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProjectRE.h"
#include "InteractableTrace.h"
#include "ClimbingLadder.h"
#include "GameplayUtils.h"
#include "Health.h"
#include "InventoryComponent.h"
#include "MasterUnlockDoor.h"
#include "ThroughTightSpaces.h"
#include "VaultObstacle.h"
#include "WeaponBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MotionWarpingComponent.h"
#include "Components/AudioComponent.h"

AProjectReCharacter::AProjectReCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Interactable Trace Component
	InteractableTraceComp = CreateDefaultSubobject<UInteractableTrace>("Interactable Trace Component");
	
	// Climbing Ladder Component
	ClimbingLadderComponent = CreateDefaultSubobject<UClimbingLadder>("Climbing Ladder Component");
	
	// Tight Spaces Component
	TightSpacesComp = CreateDefaultSubobject<UThroughTightSpaces>("Tight Spaces Component");
	
	// Inventory Component
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
	
	// Health Component
	HealthComp = CreateDefaultSubobject<UHealth>("Health Component");
	
	// Vault Obstacle Component
	VaultObstacleComp = CreateDefaultSubobject<UVaultObstacle>("Vault Obstacle Component");
	
	// Motion Warping Component
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>("Motion Warping Component");
	
	//Heartbeat Audio Component
	HeartbeatSound = CreateDefaultSubobject<UAudioComponent>("Heartbeat Audio Component");
	
	//Footstep Audio Component
	FootstepAudioComp = CreateDefaultSubobject<UAudioComponent>("Footstep Audio Component");
	FootstepAudioComp->SetupAttachment(GetMesh());
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AProjectReCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectReCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AProjectReCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectReCharacter::Look);
		
		// Interact
		EnhancedInputComponent->BindAction(InteractingAction, ETriggerEvent::Started, this, &AProjectReCharacter::Interact);
		
		//Hotbar
		EnhancedInputComponent->BindAction(Slot1Action, ETriggerEvent::Started, this, &AProjectReCharacter::EquipWeaponSlot1);
		EnhancedInputComponent->BindAction(Slot2Action, ETriggerEvent::Started, this, &AProjectReCharacter::EquipWeaponSlot2);
		EnhancedInputComponent->BindAction(Slot3Action, ETriggerEvent::Started, this, &AProjectReCharacter::EquipWeaponSlot3);
		EnhancedInputComponent->BindAction(Slot4Action, ETriggerEvent::Started, this, &AProjectReCharacter::EquipWeaponSlot4);
		
		// Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &AProjectReCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &AProjectReCharacter::StopAiming);
		
		// Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this , &AProjectReCharacter::ShootWeapon);
		
		// Reload 
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AProjectReCharacter::ReloadWeapon);
		 
		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Started, this, &AProjectReCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction,ETriggerEvent::Completed, this, &AProjectReCharacter::StopSprint);
		
		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction,ETriggerEvent::Started, this, &AProjectReCharacter::Inventory);
		
		// Vault Obstacle
		EnhancedInputComponent->BindAction(VaultObstacleAction, ETriggerEvent::Started, this, &AProjectReCharacter::VaultObstacle);
	}
	else
	{
		UE_LOG(LogProjectRE, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectReCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
	MasterHUD = CreateWidget<UUserWidget>(GetWorld(), MasterHUDWidgetClass);
	HotbarWidget = CreateWidget<UUserWidget>(GetWorld(), MasterHotbarWidgetClass);
	InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), MasterInventoryWidgetClass);
	UnlockedDoorWidget = CreateWidget<UMasterUnlockDoor>(GetWorld(), MasterUnlockedDoorWidgetClass);
	QuestLeaderboard = CreateWidget(GetWorld(), MasterQuestLeaderboardWidgetClass);
	CurrentState = ECharacterState::ECS_Idle;
	PC = UGameplayStatics::GetPlayerController(this, 0);
	
	ClimbingLadderComponent->OnReachTopClimbing.AddDynamic(this, &AProjectReCharacter::ReachTopClimbingDispatcher);
	ClimbingLadderComponent->OnStartClimbing.AddDynamic(this, &AProjectReCharacter::StartClimbingLadderDispatcher);
	ClimbingLadderComponent->OnStopClimbing.AddDynamic(this, &AProjectReCharacter::StopClimbingLadderDispatcher);
	ClimbingLadderComponent->OnFinishedClimbing.AddDynamic(this, &AProjectReCharacter::FinishedClimbingDispatcher);
	
	TightSpacesComp->StartTightSpacesDispatcher.AddDynamic(this, &AProjectReCharacter::StartTightSpacesDispatcher);
	TightSpacesComp->StopTightSpacesDispatcher.AddDynamic(this, &AProjectReCharacter::StopTightSpacesDispatcher);
	
	VaultObstacleComp->OnFinishVaulting.AddDynamic(this, &AProjectReCharacter::FinishVaulting);
	VaultObstacleComp->OnMantleVault.AddDynamic(this, &AProjectReCharacter::MantleAnimation);
	VaultObstacleComp->OnVault.AddDynamic(this, &AProjectReCharacter::VaultingAnimation);
	
	InventoryComp->OnEquipWeaponAnimation.AddDynamic(this, &AProjectReCharacter::EquipWeaponAnimation);
	InventoryComp->OnCloseUnlockDoorUI.AddDynamic(this, &AProjectReCharacter::CloseUnlockDoorUI);
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AProjectReCharacter::PlayHeartbeat);
	
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
	
	if (MasterHUD)
	{
		MasterHUD->AddToViewport();
	}
	
	if (HotbarWidget)
	{
		HotbarWidget->AddToViewport();
		HotbarWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (InventoryWidget)
	{
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (UnlockedDoorWidget)
	{
		UnlockedDoorWidget->AddToViewport();
		UnlockedDoorWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (QuestLeaderboard)
	{
		QuestLeaderboard->AddToViewport();
	}
}

void AProjectReCharacter::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	const FRotator CameraRot = GetControlRotation();
	const FRotator PlayerRot = GetActorRotation();

	FRotator DesiredHeadRot = PlayerRot - CameraRot;
	DesiredHeadRot.Normalize();

	const float InvertPitchRot = DesiredHeadRot.Pitch * 1.0f;
	const float InvertYawRot = DesiredHeadRot.Yaw * -1.0f;
	
	float PitchRotClamp;
	float YawRotClamp;
	
	if (FMath::Abs(DesiredHeadRot.Yaw) > 110.0f)
	{
		PitchRotClamp = 0.0f;
		YawRotClamp = 0.0f;
	}
	else
	{
		PitchRotClamp = FMath::Clamp(InvertPitchRot, -60.0f, 60.0f);
		YawRotClamp = FMath::Clamp(InvertYawRot, -80.0f, 80.0f);
	}

	const FRotator TargetRot = FRotator(YawRotClamp, PitchRotClamp, 0.0f);
	constexpr float InterpSpeed = 15.0f; 

	FinalHeadRot = FMath::RInterpTo(FinalHeadRot, TargetRot, DeltaSeconds, InterpSpeed);
	
	//Set The Current Weapon Recoil to Last Weapon Recoil To Use It Later
	//Reminder Need To Save Last Weapon Recoil Early on So It Doesn't Get Overwrite
	const float LastWeaponRecoil = CurrentWeaponRecoil;
	                                                       
	//Reminder Interp Calculation (Current Weapon Recoil + (Weapon Recoil Target - Current Weapon Recoil) x Delta Seconds x Recoil Speed)
	//Need To Subtract These (Weapon Recoil Target - Current Weapon Recoil) First
	//Then Saved The Value To Current Weapon Recoil (Before The =) and Use It To Calculated Final Weapon Recoil
	CurrentWeaponRecoil = FMath::FInterpTo(CurrentWeaponRecoil, WeaponRecoilTarget, DeltaSeconds, RecoilSpeed);
	
	//We Subtract Current Weapon Recoil With Last Weapon Recoil To Get The Value For Add Controller Pitch Input
	//We Need This Because Add Controller Pitch Input Is Stackable and When Subtract Current Weapon Recoil and Last Weapon Recoil 
	//We Get The Real Value To Hit The Weapon Recoil Target
	const float FinalWeaponRecoil = CurrentWeaponRecoil - LastWeaponRecoil;
	
	AddControllerPitchInput(-FinalWeaponRecoil);
}

#pragma region Player Input
void AProjectReCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AProjectReCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AProjectReCharacter::DoMove(const float Right, const float Forward)
{
	if (TightSpacesComp->GetIsThroughTightSpaces())
	{
		TightSpacesComp->TightSpacesMovement(Forward);
		return;
	}
	
	if (ClimbingLadderComponent->GetIsClimbing())
	{
		ClimbingLadderComponent->ClimbingMovement(Forward);
		return;
	}
	
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AProjectReCharacter::DoLook(const float Yaw, const float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AProjectReCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AProjectReCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
#pragma endregion

#pragma region ClimbingLadder Event Dispatcher
void AProjectReCharacter::StartClimbingLadderDispatcher()
{
	CurrentState = ECharacterState::ECS_ClimbingLadder;
}

void AProjectReCharacter::StopClimbingLadderDispatcher()
{
	CurrentState = ECharacterState::ECS_Idle;
}

void AProjectReCharacter::ReachTopClimbingDispatcher()
{
	DisableInput(PC);
}

void AProjectReCharacter::FinishedClimbingDispatcher()
{
	EnableInput(PC);
}
#pragma endregion

#pragma region Weapon Functions
void AProjectReCharacter::EquipWeaponSlot1()
{
	SwitchingWeapons(1);
}

void AProjectReCharacter::EquipWeaponSlot2()
{
	SwitchingWeapons(2);
}

void AProjectReCharacter::EquipWeaponSlot3()
{
	SwitchingWeapons(3);
}

void AProjectReCharacter::EquipWeaponSlot4()
{
	SwitchingWeapons(4);
}

void AProjectReCharacter::SwitchingWeapons(const int32 NumberSlot)
{
	if (!CanDoActions()) return;
	// Player Can Only Change Weapon When CharacterState Is Idle/Walking
	if (CurrentState != ECharacterState::ECS_Idle) return;
	if (!InventoryComp->CheckEquipWeapon(NumberSlot)) return;
	if (!InventoryComp->GetEquippedGun()) return;
	if (!InventoryComp->GetEquippedGun()->GetHolsterAudio()) return;
	
	//Play Current Equipped Gun Holster Audio 
	InventoryComp->GetEquippedGun()->GetHolsterAudio()->Play();
	
	//If True Set The Left Hand Based On The Selected Weapon On Slots
	SetLeftHandIKToBarrel();
	
	//Update Inventory Variables For Player Animation
	OnUpdateInventoryForAnimation.Broadcast();
}

void AProjectReCharacter::SetLeftHandIKToBarrel()
{
	const FTransform RightHand = GetMesh()->GetSocketTransform("hand_r");
	const FVector BarrelLocation = InventoryComp->GetEquippedGun()->GetLeftHandSocket()->GetComponentLocation();
	const FVector WeaponLocalOffset = RightHand.InverseTransformPosition(BarrelLocation);
			
	//Adding IK Hand Location For Each Weapon Into TMap For Left Hand When Holding a Weapon
	//IKHandHoldWeapon is Used Inside Player Animation Blueprint
	IKHandHoldWeapon = WeaponLocalOffset;
}

void AProjectReCharacter::EquipWeaponAnimation(UAnimMontage* EquipWeaponAnimation)
{
	if (!EquipWeaponAnimation)
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Forgot To Put Animation On Weapon"),this);
		return;
	}
	
	PlayAnimMontage(EquipWeaponAnimation);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	FTimerHandle EquipAnimationTimerHandle;
	GetWorldTimerManager().SetTimer(EquipAnimationTimerHandle, [this]
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	},
	EquipWeaponAnimation->GetPlayLength(),
	false
	);
}

void AProjectReCharacter::StartAiming()
{ 
	if (!CanDoActions()) return;
	if (!InventoryComp->GetEquippedGun()) return;
	
	AWeaponBase* WeaponRef = InventoryComp->GetEquippedGun();
	
	WeaponRef->StartAimingWeapon();
	
	// This Will Called An Event Inside Player Character Blueprint
	StartUpdateCameraAiming();
	
	//Player Camera Function When Aiming
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	CameraManager->ViewPitchMin = WeaponRef->GetMinAimingClamp();
	CameraManager->ViewPitchMax =  WeaponRef->GetMaxAimingClamp();
	GetCharacterMovement()->MaxWalkSpeed =  WeaponRef->GetAimingWalkSpeed();
	CurrentState = ECharacterState::ECS_Aiming;
	
	//When Aiming Weapon, Player Only Rotate Yaw Axis
	const FRotator YawRotation = GetActorRotation();
	const FRotator FinalYawRotation = FRotator(0, YawRotation.Yaw, 0);
	SetActorRotation(FinalYawRotation);
	
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AProjectReCharacter::StopAiming()
{
	if (!InventoryComp->GetEquippedGun()) return;
	
	AWeaponBase* WeaponRef = InventoryComp->GetEquippedGun();
	WeaponRef->StopAimingWeapon();
	
	// This Will Called An Event Inside Player Character Blueprint
	StopUpdateCameraAiming();
	
	//Player Camera Function When Aiming
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	CameraManager->ViewPitchMin = -90.0f;
	CameraManager->ViewPitchMax = 90.0f;
	GetCharacterMovement()->MaxWalkSpeed =  250.0f;
	CurrentState = ECharacterState::ECS_Idle;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AProjectReCharacter::ShootWeapon()
{
	if (!InventoryComp->GetEquippedGun()) return;

	const FVector StartLoc = GetFollowCamera()->GetComponentLocation();
	const FVector ForwardLoc = GetFollowCamera()->GetForwardVector();
	
	// Check If Weapon Actually Fired For Weapon Camera Shake To Start
	const bool bSuccessFire = InventoryComp->GetEquippedGun()->FireWeapon(StartLoc, ForwardLoc);
	
	if (bSuccessFire)
	{
		WeaponCameraShake();
	}
}

void AProjectReCharacter::CameraWeaponRecoil_Implementation(const float CurrentRecoilWeapon)
{
	//Added Current Recoil Weapon To Weapon Recoil Target
	//Reminder Adding Doesn't Overwrite The Original Value 
	//Because We Still Get The Weapon Recoil Target Before It Get Added By Current Recoil Weapon (10, 20, 30...)
	WeaponRecoilTarget += CurrentRecoilWeapon;
	
	//Everytime Camera Weapon Recoil is Called It Will Reset The Timer and Start The Timer Again
	GetWorldTimerManager().ClearTimer(CameraRecoilTimerHandle);
	GetWorldTimerManager().SetTimer(CameraRecoilTimerHandle, [this]
	{
		WeaponRecoilTarget = 0;
		CurrentWeaponRecoil = 0;
	},
	0.2f,
	false
	);
}

void AProjectReCharacter::WeaponCameraShake() const
{
	// Only Camera Shake Everytime Player Able To Shoot
	if (InventoryComp->GetEquippedGun()->GetCameraShakeClass())
	{
		PC->ClientStartCameraShake(InventoryComp->GetEquippedGun()->GetCameraShakeClass());
	}
}

void AProjectReCharacter::ReloadWeapon()
{
	if (!CanDoActions()) return;
	if (!InventoryComp->GetEquippedGun()) return;
	if (!InventoryComp->CalculatedNeedAmmo()) return;
	
	PlayAnimMontage(InventoryComp->GetEquippedGun()->GetReloadAnimation());
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	InventoryComp->GetEquippedGun()->GetReloadAudio()->Play();
	CurrentState = ECharacterState::ECS_Reloading;
	InventoryComp->GetEquippedGun()->SetCurrentlyReloading(true);
}

void AProjectReCharacter::AfterReload()
{
	//Called By Event Notify After Finishing Reload Animation For Each Weapon
	CurrentState = ECharacterState::ECS_Idle;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

#pragma endregion

#pragma region Momentum Movement System
void AProjectReCharacter::StartSprint()
{
	if (!CanDoActions()) return;
	
	bSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AProjectReCharacter::StopSprint()
{
	bSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}

void AProjectReCharacter::OnMovementModeChanged(const EMovementMode PrevMovementMode, const uint8 PreviousCustomMode)
{
	// Checking If Player Is Currently Falling Or not In Real Time and Change The Current State To Falling
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		CurrentState= ECharacterState::ECS_Falling;
	}
}

void AProjectReCharacter::Landed(const FHitResult& Hit)
{
	
	Super::Landed(Hit);
	
	if (CurrentState == ECharacterState::ECS_Falling)
	{
		CurrentState= ECharacterState::ECS_Idle;
	}
}

void AProjectReCharacter::PlayFootstepSound()
{
	const FVector PlayerStep = GetActorLocation() + (-GetActorUpVector() * 100);
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	
	//The Hit Result Could Be Hitting The Actor Itself So It Will Get The Actor Material Instead Of Ground
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), PlayerStep, ECC_Visibility, QueryParams);
	
	// Checking If the Line Trace Hit Anything Based On The Query and Checking If The Hit Result PhysMaterial Is Valid
	if (bHit)
	{
		//Check Physical Material Surface Type (Can Be Change Inside Physics Material Setting)
		const EPhysicalSurface SurfaceType = HitResult.PhysMaterial->SurfaceType;
		if (!SurfaceType) return;
		
		// Check If The TMap Contains The Type Surface That We Give
		if (!FootstepSound.Contains(SurfaceType))
		{
			UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Doesn't Contain This Type Of Sound Or Material Inside TMap"), this);
			return;
		}
		// Set The SoundBaseOnSurface Based On The TMap Using Surface Type
		USoundBase* SoundBaseOnSurface = FootstepSound[SurfaceType];
	
		// Play The Sound On Impact Point Of The Line Trace
		if (!IsValid(FootstepAudioComp)) return;
		FootstepAudioComp->SetSound(SoundBaseOnSurface);
		FootstepAudioComp->Play();
	}
}
#pragma endregion 

#pragma region Through Tight Space Dispatcher
void AProjectReCharacter::StartTightSpacesDispatcher()
{
	CurrentState = ECharacterState::ECS_TightSpace;
	PC->SetIgnoreLookInput(true);
	TightSpacesComp->GetTightSpaceActor()->bFindCameraComponentWhenViewTarget = true;
	PC->SetViewTargetWithBlend(TightSpacesComp->GetTightSpaceActor(), 0.5f, VTBlend_Linear);
}

void AProjectReCharacter::StopTightSpacesDispatcher()
{
	CurrentState = ECharacterState::ECS_Idle;
	PC->SetIgnoreLookInput(false);
	PC->bFindCameraComponentWhenViewTarget = true;
	PC->SetViewTargetWithBlend(this, 0.5f, VTBlend_Linear);
}
#pragma endregion 

#pragma region Vault Obstacle Dispatcher
void AProjectReCharacter::VaultObstacle()
{
	if (!CanDoActions()) return;
	if (bAlreadyVaulted) return;
	
	if (VaultObstacleComp)
	{
		VaultObstacleComp->VaultObstacle();
	}
}

void AProjectReCharacter::MantleAnimation(const FVector& MantleLocation, UAnimMontage* MantleAnimation)
{
	//To Prevent Spamming Vault Action
	bAlreadyVaulted = true;
	
	GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(FName("Mantle"), MantleLocation);
	PlayAnimMontage(MantleAnimation);
	
	// Set The Player Movement To Flying So the Player Doesn't Get Pull Down By The Gravity (If I Don't Do This The Player Stuck At One Place)
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	
	// Set The Player Capsule To Ignore WorldStatic So The Player Can Mantle Or Vault Without Getting Stuck On The Static Object
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
}

void AProjectReCharacter::VaultingAnimation(const FVector& VaultLocation, UAnimMontage* VaultAnimation)
{
	bAlreadyVaulted = true;
	
	GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(FName("Vault"), VaultLocation);
	PlayAnimMontage(VaultAnimation);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	ControlRig = false;
	GetCameraBoom()->bDoCollisionTest = false;
	
	//Update Vaulting Variables For Player Animation
	OnUpdateVaultingForAnimation.Broadcast(ControlRig);
}

void AProjectReCharacter::FinishVaulting()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	// On Each Animation There's AnimNotify Where It Will Call This Function On Certain Frame
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCameraBoom()->bDoCollisionTest = true;
	ControlRig = true;
	bAlreadyVaulted = false;
	
	//Update Vaulting Variables For Player Animation
	OnUpdateVaultingForAnimation.Broadcast(ControlRig);
}

void AProjectReCharacter::GetVaultingHandPlacement()
{
	if (!VaultObstacleComp) return;
	
	IKHandInteractable = VaultObstacleComp->VaultingHandPlacement();
	OnUpdateInteractableForAnimation.Broadcast();
}

USkeletalMeshComponent* AProjectReCharacter::GetCharacterSkeletalMeshComponent_Implementation()
{
	return GetMesh();
}

void AProjectReCharacter::SetCharacterState_Implementation(const ECharacterState NewCharacterState)
{
	CurrentState = NewCharacterState;
}

ECharacterState AProjectReCharacter::GetCharacterState_Implementation()
{
	return CurrentState;
}

bool AProjectReCharacter::GetIsSprinting_Implementation()
{
	return GetSprinting();
}

#pragma endregion

#pragma region UnlockDoor UI
void AProjectReCharacter::DoorUnlockMode_Implementation(AActor* DoorActor)
{
	if (!IsValid(DoorActor)) return;
	
	// Switch The Current Player Camera To Interact Door Camera
	PC->SetViewTargetWithBlend(DoorActor , 0.5f, VTBlend_Cubic);
	
	//Everytime Interact With The Unlock Door It Will Open The UI
	OpenUnlockDoorUI();
	
	//Broadcast To Bind To UnlockDoorSlot To Set Current Interact Door Actor 
	OnCurrentInteractDoor.Broadcast(DoorActor);
}

void AProjectReCharacter::OpenUnlockDoorUI() const
{
	if (!IsValid(UnlockedDoorWidget))
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Unlock Door Widget Is Null"), this);
		return;
	}
	
	//Set Game To UI and Game Mode Only
	const FInputModeGameAndUI UIMode;
	PC->SetInputMode(UIMode);
	PC->bShowMouseCursor = true;
	
	//Disable Player Movement So Player Can't Move While In Unlock Door Mode UI
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	if (InventoryComp->GetEquippedGun())
	{
		//Set Back Weapon Mesh To Visible 
		InventoryComp->GetEquippedGun()->GetWeaponMesh()->SetVisibility(false);
	}
	//Set Back Player To Visible
	GetMesh()->SetVisibility(false);
	
	//Make Unlock Door UI Visible To The Screen
	UnlockedDoorWidget->SetVisibility(ESlateVisibility::Visible);
}

void AProjectReCharacter::CloseUnlockDoorUI()
{
	if (!IsValid(UnlockedDoorWidget))
	{
		UGameplayUtils::PrintDebug(2.0f, FColor::Red, TEXT("Unlock Door Widget or Equipped Gun Is Null"), this);
		return;
	}
	
	// Switch Back To Player Camera When Close The UI
	PC->SetViewTargetWithBlend(this, 0.5f, VTBlend_Cubic);
	
	// When Close UI It Will Clear Icon and Name
	UnlockedDoorWidget->ClearIconAndName();
	
	//Set Game Back To Game Mode Only
	const FInputModeGameOnly GameMode;
	PC->SetInputMode(GameMode);
	PC->bShowMouseCursor = false;
	
	//Enable Player Movement Again
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	if (InventoryComp->GetEquippedGun())
	{
		//Set Back Weapon Mesh To Visible
		InventoryComp->GetEquippedGun()->GetWeaponMesh()->SetVisibility(true);
	}
	
	//Set Back Player To Visible
	GetMesh()->SetVisibility(true);
	
	//Make Unlock Door UI Invisible To The Screen
	UnlockedDoorWidget->SetVisibility(ESlateVisibility::Collapsed);
}
#pragma endregion

#pragma region Interactable
void AProjectReCharacter::Interact()
{
	if (!CanDoActions()) return;
	if (!IsValid(InteractableTraceComp)) return;
	
	// Everytime Interact Key Is Pressed Line Trace Will Shoot Instead Of Every Tick
	const FVector StartTraceLocation = GetFollowCamera()->GetComponentLocation();
	const FVector ForwardTraceLocation = GetFollowCamera()->GetForwardVector();
	
	InteractableTraceComp->TraceTimerScanner(StartTraceLocation, ForwardTraceLocation);
}

void AProjectReCharacter::PickableItemIK_Implementation(const FVector& PickableItemLocation)
{
	if (!InteractAnimation) return;
	
	//Set IKHandInteractable Based On The Interact Actor Location
	//IKHandInteractable Is Used Inside Player Animation Blueprint
	IKHandInteractable = PickableItemLocation;
	PlayAnimMontage(InteractAnimation);
	CurrentState = ECharacterState::ECS_Interact;
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
	
	//Update Interactable Variables For Player Animation
	OnUpdateInteractableForAnimation.Broadcast();
}
#pragma endregion

void AProjectReCharacter::Inventory()
{
	if (!HotbarWidget) return;
	if (!InventoryWidget) return;
	
	if (!bOpenInventory)
	{
		const FInputModeGameAndUI UIMode;
		HotbarWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PC->SetInputMode(UIMode);
		PC->bShowMouseCursor = true;
		bOpenInventory = true;
	}
	else
	{
		const FInputModeGameOnly GameMode;
		HotbarWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		PC->SetInputMode(GameMode);
		PC->bShowMouseCursor = false;
		bOpenInventory = false;
	}
}

void AProjectReCharacter::ReEnableAllInput()
{
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);
	CurrentState = ECharacterState::ECS_Idle;
}

void AProjectReCharacter::Died_Implementation()
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	
	if (!PC) return;
	PC->UnPossess();
}

void AProjectReCharacter::HitCameraShake_Implementation()
{
	if (HitCameraShake)
	{
		PC->ClientStartCameraShake(HitCameraShake);
	}
}

FGenericTeamId AProjectReCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(0);
}

void AProjectReCharacter::PlayHeartbeat(const float CurrentHealth, const bool IsAlive)
{
	if (!IsValid(HeartbeatSound)) return;
	
	if (CurrentHealth <= 20.0f)
	{
		HeartbeatSound->Play();
	}
	else if (!IsAlive)
	{
		HeartbeatSound->Stop();
	}
}

bool AProjectReCharacter::CanDoActions() const
{
	switch (CurrentState)
	{
	case ECharacterState::ECS_ClimbingLadder:
	case ECharacterState::ECS_TightSpace:
	case ECharacterState::ECS_Reloading:
	case ECharacterState::ECS_Falling:
	case ECharacterState::ECS_Interact:
	case ECharacterState::ECS_Aiming:	
		return false;
		
	case ECharacterState::ECS_Idle:
		return true;
		
	default:
		return true;
	}
}