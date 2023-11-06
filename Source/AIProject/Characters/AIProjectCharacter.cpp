// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
//#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "AIProject/Components/HealthComponent.h"
#include "AIProject/Components/MeleeComponent.h"
#include "AIProject/Controllers/BasicAIController.h"


//////////////////////////////////////////////////////////////////////////
// AAIProjectCharacter

AAIProjectCharacter::AAIProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a health component (handles hp and death)
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	//HealthComponent->SetupAttachment(RootComponent);

	// Create a melee component (handles melee attacks)
	MeleeComponent = CreateDefaultSubobject<UMeleeComponent>(TEXT("MeleeComponent"));
	//HealthComponent->SetupAttachment(RootComponent);


	//// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	//
	//// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AAIProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	/// debug msg
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Yellow,
	//		FString(TEXT("AI Project Character"))
	//	);
	//}

	if (bDebugMsg && GEngine)
	{
		FString actionState = UEnum::GetValueAsString(GetActionState());
		//FString debugMessage = FString::Printf(TEXT("Current action state is: %s"), actionState);
		GEngine->AddOnScreenDebugMessage(
			2,
			5.f,
			FColor::Green,
			//FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetOwner()->GetName())
			actionState
		);
	}
}

void AAIProjectCharacter::Tick(float DeltaSeconds)
{
	//if (bDebugMsg && GEngine)
	//{
	//	FString debugMessage = FString::Printf(TEXT("Current action state is: %s"), GetActionState());
	//	GEngine->AddOnScreenDebugMessage(
	//		2,
	//		1.f,
	//		FColor::Red,
	//		//FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetOwner()->GetName())
	//		debugMessage
	//	);
	//	}

}

void AAIProjectCharacter::SetActionState(EActionState NewState)
{
	FString prevState = UEnum::GetValueAsString(GetActionState());

	ActionState = NewState;

	// show state transition
	if (bDebugMsg && GEngine)
	{
		FString currentState = UEnum::GetValueAsString(GetActionState());
		FString msg = "EActionState transition: " + prevState + " -> " + currentState;
		//FString debugMessage = FString::Printf(TEXT("Current action state is: %s"), actionState);
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			//FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetOwner()->GetName())
			msg
		);
	}

	// update blackboards (if character is an AI)
	if (AController* owningController = GetController())
	{
		if (ABasicAIController* AIController = Cast<ABasicAIController>(owningController))
		{
			AIController->UpdateBlackboard();
		}
	}
}

void AAIProjectCharacter::CheckActionState()
{
	if (bDebugMsg && GEngine)
	{
		FString currentState = UEnum::GetValueAsString(GetActionState());
		FString msg = "Current Action State: " + currentState;
		GEngine->AddOnScreenDebugMessage(
			5,
			5.f,
			FColor::Yellow,
			msg
		);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AAIProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAIProjectCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAIProjectCharacter::Look);

		//Test Something
		EnhancedInputComponent->BindAction(TestAction1, ETriggerEvent::Triggered, this, &AAIProjectCharacter::TestFunction1);
		EnhancedInputComponent->BindAction(TestAction2, ETriggerEvent::Triggered, this, &AAIProjectCharacter::TestFunction2);
		EnhancedInputComponent->BindAction(TestAction3, ETriggerEvent::Triggered, this, &AAIProjectCharacter::TestFunction3);
	}

}

void AAIProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAIProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAIProjectCharacter::TestFunction1(const FInputActionValue& Value)
{
	/// debug msg
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			1.f,
			FColor::Yellow,
			FString(TEXT("Testing Function 1..."))
		);
	}

	// perform a melee attack
	MeleeComponent->PerformBasicAttack();

	// track the character's hand
	//MeleeComponent->DrawRadialAtk();
}

void AAIProjectCharacter::TestFunction2(const FInputActionValue& Value)
{
	/// debug msg
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			1.f,
			FColor::Yellow,
			FString(TEXT("Testing Function 2..."))
		);
	}

	CheckActionState();
}

void AAIProjectCharacter::TestFunction3(const FInputActionValue& Value)
{
	/// debug msg
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			1.f,
			FColor::Yellow,
			FString(TEXT("Testing Function 3..."))
		);
	}
}