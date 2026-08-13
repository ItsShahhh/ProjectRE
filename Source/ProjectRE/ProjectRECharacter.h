// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterState.h"
#include "DiedInterface.h"
#include "GenericTeamAgentInterface.h"
#include "DamageableInterface.h"
#include "InteractableInterface.h"
#include "PlayerStateInterface.h"
#include "LockableInterfaces.h"
#include "ProjectRECharacter.generated.h"

#pragma region  ForwardDeclared
class UQuestSystem;
class UMotionWarpingComponent;
class UUnlockedDoorSlots;
class UMasterUnlockDoor;
class AWeaponBase;
class UHotbarData;
class UVaultObstacle;
class UHealth;
class UMasterHotbar;
class UInventoryComponent;
class UClimbingLadder;
class USpringArmComponent;
class UCameraComponent;
class UInteractableTrace;
class UInputAction;
class UThroughTightSpaces;
struct FInputActionValue;
#pragma endregion 

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentInteractDoor, AActor*, DoorActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateInventoryForAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateInteractableForAnimation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateVaultingForAnimation, const bool, bControlRig);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AProjectReCharacter : public ACharacter, public IDiedInterface, public IGenericTeamAgentInterface, public IDamageableInterface, public IPlayerStateInterface, public ILockableInterfaces, public IInteractableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	// LineTrace Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UInteractableTrace* InteractableTraceComp;
	
	// ClimbingLadder Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UClimbingLadder* ClimbingLadderComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UThroughTightSpaces* TightSpacesComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UHealth* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UVaultObstacle* VaultObstacleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComp;

protected:
	/** Jump to Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractingAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Slot1Action;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Slot2Action;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Slot3Action;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* Slot4Action;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AimingAction; 
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ReloadAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InventoryAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* VaultObstacleAction;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MasterHUDWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MasterHotbarWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MasterInventoryWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MasterUnlockedDoorWidgetClass;
	
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> MasterQuestLeaderboardWidgetClass;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UMasterUnlockDoor* UnlockedDoorWidget;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UUserWidget* CrosshairWidget;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UUserWidget* HotbarWidget;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UUserWidget* InventoryWidget;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UUserWidget* MasterHUD;
	
	UPROPERTY(VisibleAnywhere, Category="UI")
	UUserWidget* QuestLeaderboard;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	TMap<TEnumAsByte<EPhysicalSurface>,USoundBase*> FootstepSound;
	
	UPROPERTY(EditAnywhere, Category="CameraShake")
	TSubclassOf<UCameraShakeBase>HitCameraShake;
	
	//For Player Only
	UPROPERTY(EditAnywhere, Category= "Health")
	UAudioComponent* HeartbeatSound;
	
	UPROPERTY(BlueprintReadWrite)
	FRotator FinalHeadRot;
	
	UPROPERTY(VisibleAnywhere, Category="State")
	ECharacterState CurrentState;
	
	UPROPERTY(EditAnywhere, Category= "Animation")
	UAnimMontage* InteractAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation")
	FVector IKHandInteractable;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation")
	FVector IKHandHoldWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Animation")
	bool ControlRig = true;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateInteractableForAnimation OnUpdateInteractableForAnimation;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateVaultingForAnimation OnUpdateVaultingForAnimation;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpdateInventoryForAnimation OnUpdateInventoryForAnimation;
	
private:
	//Variables
	bool bSprinting = false;
	bool bOpenInventory = false;
	
	EPhysicalSurface SurfaceType;
	FVector FootstepLocation;
	
	UPROPERTY()
	APlayerController* PC;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	// Called For Interact
	void Interact();
	
	// Called For Equip Gun
	void EquipWeaponSlot1();
	void EquipWeaponSlot2();
	void EquipWeaponSlot3();
	void EquipWeaponSlot4();
	void SwitchingWeapons(int32 NumberSlot);
	void SetLeftHandIKToBarrel();
	
	// Aiming
	void StartAiming();
	void StopAiming();
	
	// Shoot
	void ShootWeapon();
	
	// Weapon Camera Shake
	void WeaponCameraShake() const;
	
	// Reload
	void ReloadWeapon();
	
	// Sprinting
	void StartSprint();
	void StopSprint();
	
	// Inventory
	void Inventory();
	
	// Vault Obstacle
	void VaultObstacle();
	
	// Footstep Sound
	UFUNCTION(BlueprintCallable)
	void PlayFootstepSound();

	// Unlocking Door Dispatcher
	UFUNCTION()
	void CloseUnlockDoorUI();
	
	//Unlock Door
	void OpenUnlockDoorUI() const;
	
	// Dispatcher
	// Climbing Ladder Dispatcher
	UFUNCTION()
	void StartClimbingLadderDispatcher();
	UFUNCTION()
	void StopClimbingLadderDispatcher();
	UFUNCTION()
	void ReachTopClimbingDispatcher();
	UFUNCTION()
	void FinishedClimbingDispatcher();
	
	// Through Tight Spaces Dispatcher
	UFUNCTION()
	void StartTightSpacesDispatcher();
	UFUNCTION()
	void StopTightSpacesDispatcher();
	
	// Vault Obstacle Dispatch
	UFUNCTION()
	void CheckVaultingState();
	UFUNCTION()
	void FinishVaulting();
	UFUNCTION()
	void MantleAnimation(const FVector& MantleLocation, UAnimMontage* MantleAnimation);
	UFUNCTION()
	void VaultingAnimation(const FVector& VaultLocation, UAnimMontage* VaultAnimation);
	
	//Reload Dispatch
	UFUNCTION(BlueprintCallable)
	void AfterReload();
	
	// Equip Weapon Animation Dispatch
	UFUNCTION()
	void EquipWeaponAnimation(UAnimMontage* EquipWeaponAnimation);
	
	//Heartbeat Sound
	UFUNCTION()
	void PlayHeartbeat(const float CurrentHealth, const bool IsAlive);
	
	// Perform Action
	bool CanDoActions() const;
	
public:
	
	/** Constructor */
	AProjectReCharacter();	
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartUpdateCameraAiming();
	UFUNCTION(BlueprintImplementableEvent)
	void StopUpdateCameraAiming();
	
	//Event Tick
	virtual void Tick(float DeltaSeconds) override;
	
	// Item IK Interact
	UFUNCTION()
	virtual void PickableItemIK_Implementation(const FVector& PickableItemLocation) override;
	
	// Death
	virtual void Died_Implementation() override;
	
	// Function To Give Team ID For Detection For Affiliation in AI Perception
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	// Falling
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
	// Land
	virtual void Landed(const FHitResult& Hit) override;
	
	//Enemy Hit Camera Shake
	virtual void HitCameraShake_Implementation() override;
	
	//Unlock Door
	UFUNCTION()
	virtual void DoorUnlockMode_Implementation(AActor* DoorActor) override;
	
	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	
	// Re-Enable Input After Interact
	UFUNCTION(BlueprintCallable)
	void ReEnableAllInput();
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCurrentInteractDoor OnCurrentInteractDoor;
	
	FORCEINLINE bool GetControlRig() const { return ControlRig; }
	
	FORCEINLINE UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComp;}

	FORCEINLINE bool GetSprinting() const { return bSprinting; }
	
	FORCEINLINE UUserWidget* GetInventoryWidget() const { return HotbarWidget; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

