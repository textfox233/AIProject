// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Net/UnrealNetwork.h"
#include "AIProject/Interfaces/InteractionInterface.h"
#include "AIProject/Interactable/PlayerFlashlight.h"
#include "AIProject/Items/Weapons/Weapon.h"

//#include "Projectiles/Projectile.h"

#include "Blueprint/UserWidget.h"


APlayerCharacter::APlayerCharacter()
	: AAIProjectCharacter()
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default value for the MaxWalkSpeed of the playable character
	GetCharacterMovement()->MaxWalkSpeed = 350.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Default value for EquippedFlashlight to ensure engine does not crash
	EquippedFlashlight = nullptr;

	// Set default values for bCameraMoving & bLeftCamera bools
	bCameraMoving = false;
	bLeftCamera = false;

}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Ensure WeaponClass is not nullptr
	if (WeaponClass)
	{
		// Calculate the spawn transform (location, rotation, scale) for the weapon.
		FTransform SpawnTransform = FTransform::Identity;

		// Spawn the weapon actor
		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnTransform);

		if (SpawnedWeapon)
		{
			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (CharacterMesh)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
				SpawnedWeapon->AttachToComponent(CharacterMesh, AttachmentRules, WeaponSocketRight);
			}
		}
	}

	// Check to see if client has authority (i.e is server)
	// Good series on this: https://www.youtube.com/watch?v=TbaOyvWfJE0&list=PLZlv_N0_O1gYwhBTjNLSFPRiBpwe5sTwc&index=1
	if (HasAuthority())
	{
		// Ensure Flashlight class has been set
		if (FlashlightClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			EquippedFlashlight = GetWorld()->SpawnActor<APlayerFlashlight>(FlashlightClass, SpawnParams);
			if (EquippedFlashlight)
			{
				EquippedFlashlight->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("flashlight_socket"));
			}
		}
	}

	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this, FName("MoveCameraUpdate"));

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("MoveCameraFinished"));

	MoveCameraTimeline.AddInterpFloat(CameraCurve, ProgressUpdate);
	MoveCameraTimeline.SetTimelineFinishedFunc(FinishedEvent);

	FOnTimelineFloat FOVProgressUpdate;
	FOVProgressUpdate.BindUFunction(this, FName("ChangeFOVUpdate"));

	FOnTimelineEvent FOVFinishedEvent;
	FOVFinishedEvent.BindUFunction(this, FName("ChangeFOVFinished"));

	AimCameraTimeline.AddInterpFloat(AimCurve, FOVProgressUpdate);
	AimCameraTimeline.SetTimelineFinishedFunc(FOVFinishedEvent);

	// Get the current camera location
	StartCameraLocation = FollowCamera->GetRelativeLocation();
	DefaultFOV = FollowCamera->FieldOfView;

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCameraTimeline.TickTimeline(DeltaTime);
	AimCameraTimeline.TickTimeline(DeltaTime);

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + GetControlRotation().Vector() * 300.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(), QueryParams))
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			if (IInteractionInterface* Interface = Cast<IInteractionInterface>(Actor))
			{
				// Access the mesh component using the interface function
				UMeshComponent* MeshComponent = Interface->GetInteractableMeshComponent();
				if (MeshComponent)
				{
					// Check if the local player triggered the line trace
					if (IsLocallyControlled())
					{
						MeshComponent->SetRenderCustomDepth(true);

						// Store the highlighted actor for the current client
						HighlightedActor = Actor;

						if (!HighlightedWidgetInstance && HighlightedWidgetClass)
						{
							HighlightedWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HighlightedWidgetClass);
							if (HighlightedWidgetInstance)
							{
								HighlightedWidgetInstance->AddToViewport();
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// If the line trace doesn't hit, remove the highlight from the previous highlighted object
		if (HighlightedActor)
		{
			if (IInteractionInterface* Interface = Cast<IInteractionInterface>(HighlightedActor))
			{
				UMeshComponent* MeshComponent = Interface->GetInteractableMeshComponent();
				if (MeshComponent)
				{
					// Check if the local player triggered the line trace
					if (IsLocallyControlled())
					{
						MeshComponent->SetRenderCustomDepth(false);

						if (HighlightedWidgetInstance)
						{
							HighlightedWidgetInstance->RemoveFromParent();
							HighlightedWidgetInstance = nullptr;
						}
					}
				}
			}
			HighlightedActor = nullptr;
		}
	}

	// If the line trace doesn't hit, remove the highlight from the previously highlighted object
	if (!HitResult.GetActor() || !Cast<IInteractionInterface>(HitResult.GetActor()))
	{
		if (IInteractionInterface* Interface = Cast<IInteractionInterface>(HitResult.GetActor()))
		{
			UMeshComponent* MeshComponent = Interface->GetInteractableMeshComponent();
			if (MeshComponent)
			{
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
//////////////////////////////////////////////////////////////////////////

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Call the parent class's implementation to retain base class's bindings
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleFlashlight);

		// Aim
		EnhancedInputComponent->BindAction(AimWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AimWeapon);

		// Release Aim
		EnhancedInputComponent->BindAction(ReleaseAimWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StopAiming);

		// Camera
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleCamera);
	}

}

void APlayerCharacter::Interact()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + GetControlRotation().Vector() * 300.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(), QueryParams))
	{
		if (AActor* Actor = HitResult.GetActor())
		{
			// Check if server and call server RPC
			if (HasAuthority())
			{
				if (IInteractionInterface* Interface = Cast<IInteractionInterface>(Actor))
				{
					APlayerController* PlayerController = Cast<APlayerController>(GetController());
					Interface->Use(PlayerController);
				}
			}
			else
			{
				// Call to server
				Server_Interact();
			}
		}
	}
}

bool APlayerCharacter::Server_Interact_Validate()
{
	return true;
}

void APlayerCharacter::Server_Interact_Implementation()
{
	Interact();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerCharacter, EquippedFlashlight, COND_OwnerOnly);

		// Replicate the bIsAiming variable.
		DOREPLIFETIME(APlayerCharacter, bIsAiming);

		// Replicate the bLeftCamera variable.
		DOREPLIFETIME(APlayerCharacter, bLeftCamera);

		// Replicate the SpawnedWeapon
		DOREPLIFETIME(APlayerCharacter, SpawnedWeapon);
}

void APlayerCharacter::ToggleFlashlight()
{
	if (EquippedFlashlight)
	{
		EquippedFlashlight->ToggleFlashlight();
	}
}

void APlayerCharacter::MoveCameraUpdate(float Alpha)
{
	NewCameraLocation = StartCameraLocation;
	NewCameraLocation.Y -= 130.0f;

	FVector CameraLocation = FMath::Lerp(StartCameraLocation, NewCameraLocation, Alpha);
	FollowCamera->SetRelativeLocation(CameraLocation);
}

void APlayerCharacter::MoveCameraFinished()
{
	bCameraMoving = false;
	bLeftCamera = !bLeftCamera;

	if (bLeftCamera)
	{
		if (SpawnedWeapon)
		{
			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (CharacterMesh)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
				SpawnedWeapon->AttachToComponent(CharacterMesh, AttachmentRules, WeaponSocketLeft);
			}
		}
	}
	else
	{
		if (SpawnedWeapon)
		{
			USkeletalMeshComponent* CharacterMesh = GetMesh();
			if (CharacterMesh)
			{
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
				SpawnedWeapon->AttachToComponent(CharacterMesh, AttachmentRules, WeaponSocketRight);
			}
		}
	}
}

void APlayerCharacter::ChangeFOVUpdate(float Alpha)
{
	float NewFOV = FMath::Lerp(DefaultFOV, AimFOV, Alpha);
	FollowCamera->SetFieldOfView(NewFOV);
}

void APlayerCharacter::ChangeFOVFinished()
{

}

void APlayerCharacter::ToggleCamera()
{
	if (!bCameraMoving)
	{
		bCameraMoving = true; // Camera is in the process of moving

		if (!bLeftCamera)
		{
			MoveCameraTimeline.Play();
		}
		else
		{
			MoveCameraTimeline.Reverse();
		}
	}
}

void APlayerCharacter::AimWeapon()
{
	//GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Red, TEXT("Aiming"));
	if (HasAuthority())
	{
		bIsAiming = true;
	}
	else
	{
		Server_AimWeapon();
	}

	AimCameraTimeline.Play();
	GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
}

void APlayerCharacter::StopAiming()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Release Aim"));
	if (HasAuthority())
	{
		bIsAiming = false;
	}
	else
	{
		Server_StopAiming();
	}

	AimCameraTimeline.Reverse();
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

bool APlayerCharacter::Server_AimWeapon_Validate()
{
	return true;
}

void APlayerCharacter::Server_AimWeapon_Implementation()
{
	AimWeapon();
}

bool APlayerCharacter::Server_StopAiming_Validate()
{
	return true;
}

void APlayerCharacter::Server_StopAiming_Implementation()
{
	StopAiming();
}