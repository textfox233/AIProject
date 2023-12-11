// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIProjectCharacter.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

class IInteractInterface;
class APlayerFlashlight;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class AIPROJECT_API APlayerCharacter : public AAIProjectCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Flashlight Toggle Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FlashlightAction;

	/** Camera Toggle Shoulder Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleCameraAction;

	/** Aim weapon (Active while holding) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimWeaponAction;

	/** Release Aim weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReleaseAimWeaponAction;

	/** Fire Weapon */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireWeaponAction;

public:
	APlayerCharacter();

	// Override SetupPlayerInputComponent to add additional bindings
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// To add mapping context
	virtual void BeginPlay();

	/** Called for interaction input */
	void Interact();

	// Remote Procedure Call to ensure interactions are triggerable from all clients
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact();

	// Validation for interaction RPC
	bool Server_Interact_Validate();

	// Remote Procedure Call to ensure aiming weapon is replicated across clients
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AimWeapon();

	// Validation for aiming RPC
	bool Server_AimWeapon_Validate();

	// Remote Procedure Call to ensure that unaiming weapon is replicated across clients
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopAiming();

	// Validation for unaiming RPC
	bool Server_StopAiming_Validate();

	// Remote Procedure Call to ensure that firing weapon is replicated across clients
//	UFUNCTION(Server, Reliable, WithValidation)
//	void Server_FireWeapon();

	// Validation for firing RPC
//	bool Server_FireWeapon_Validate();

	// Remote Procedure Call to ensure spine rotation is replicated across clients
//	UFUNCTION(Server, Reliable, WithValidation)
//	void Server_SpineRotationX(const FRotator& NewSpineRotationX);

	// Validation for spine rotation RPC
//	bool Server_SpineRotationX_Validate(const FRotator& NewSpineRotationX);

	//
//	UFUNCTION(NetMulticast, Reliable, WithValidation)
//	void MulticastSpineRotationX(const FRotator& NewSpineRotationX);

	//
//	bool MulticastSpineRotationX_Validate(const FRotator& NewSpineRotationX);


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HighlightedWidgetClass;

	UUserWidget* HighlightedWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerFlashlight> FlashlightClass;

	UPROPERTY(Replicated)
	APlayerFlashlight* EquippedFlashlight;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ToggleFlashlight();

	void ToggleCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* CameraCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	UCurveFloat* AimCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimWalkSpeed = 125.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool bIsAiming;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AimWeapon();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void StopAiming();

	//	UFUNCTION(BlueprintCallable, Category = "Character")
	//	void FireWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AimFOV = 60;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool bLeftCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Weapons")
	class AWeapon* SpawnedWeapon;

	//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	//	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FRotator SpineRotationX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName WeaponSocketRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName WeaponSocketLeft;

private:

	// Stores the highlighted object
	AActor* HighlightedActor = nullptr;

	bool bCameraMoving;

	float DefaultFOV;

	FTimeline MoveCameraTimeline;
	FTimeline AimCameraTimeline;

	FVector StartCameraLocation;
	FVector NewCameraLocation;

	UFUNCTION()
	void MoveCameraUpdate(float Alpha);

	UFUNCTION()
	void MoveCameraFinished();

	UFUNCTION()
	void ChangeFOVUpdate(float Alpha);

	UFUNCTION()
	void ChangeFOVFinished();
};
