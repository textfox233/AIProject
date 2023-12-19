// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject/Interactable/SlidingDoor.h"
#include "Net/UnrealNetwork.h"
#include "Components/ArrowComponent.h"

ASlidingDoor::ASlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create DefaultSceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	DoorMesh->SetupAttachment(RootComponent);
	
	// Create the arrow component
	ForwardArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));

	// Attach the arrow component to the root component or any other component you want
	ForwardArrowComponent->SetupAttachment(RootComponent);

	DoorMesh->SetCollisionProfileName("BlockAll");
	DoorMesh->SetRenderCustomDepth(false);

	bDoorOpened = false;
	bReplicates = true;

	bIsMoving = false;
	MovementSpeed = 100.0f;
	InitialDelay = 1.0f;
	DoorTargetX = 100.0f;
}

void ASlidingDoor::Use(APlayerController* PlayerController)
{
	if (HasAuthority() && !bIsMoving)
	{
		// Set bDoorOpened to the opposite value (toggle true or false)
		bDoorOpened = !bDoorOpened;

		// Execute DoorToggled on all clients (Replication)
		OnRep_DoorToggled();
	}
}

UMeshComponent* ASlidingDoor::GetInteractableMeshComponent() const
{
	return DoorMesh;
}

void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
}

void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		MoveDoor(DeltaTime);
	}
}

void ASlidingDoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASlidingDoor, bDoorOpened);
}

void ASlidingDoor::OnRep_DoorToggled()
{
	// Start the initial delay timer to begin movement after a delay
	GetWorldTimerManager().SetTimer(TimerHandle_InitialDelay, this, &ASlidingDoor::StartMoving, InitialDelay, true);
}

void ASlidingDoor::StartMoving()
{

	// Clear any existing timers
	GetWorldTimerManager().ClearTimer(TimerHandle_InitialDelay);

	// Set the target location based on the door state
	TargetLocation = GetActorLocation() + (bDoorOpened ? -DoorTargetX * GetActorForwardVector() : DoorTargetX * GetActorForwardVector());

	// Initial delay has passed, start the movement
	bIsMoving = true;

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Server: CALLED AFTER DELAY"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Client: CALLED AFTER DELAY"));
	}

	//OnRep_DoorToggled();
}

void ASlidingDoor::MoveDoor(float DeltaTime)
{
	// Get the current location of the actor
	FVector CurrentLocation = GetActorLocation();

	// Get the forward vector of the actor (local X-axis after taking into account rotation)
	FVector ForwardVector = GetActorForwardVector();

	// Calculate the target location based on the door state
	//FVector TargetLocation = CurrentLocation + (bDoorOpened ? -DoorTargetX * ForwardVector : DoorTargetX * ForwardVector);

	// Calculate the new location by moving towards the target in local space
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MovementSpeed);

	// Check if the door has reached the target location
	if (FVector::DistSquared(NewLocation, TargetLocation) < FMath::Square(1e-3f))
	{
		// Door has reached the target location
		bIsMoving = false;

		// Reset the door position to the closed state (original position) after toggling
		if (!bDoorOpened)
		{
			SetActorLocation(TargetLocation);
		}
	}

	// Set the actor's new location
	SetActorLocation(NewLocation);

	// Debug print statements
	FString Direction = bDoorOpened ? TEXT("Closing") : TEXT("Opening");
	FString DebugMessage = FString::Printf(TEXT("%s Door: Current X = %f, Target X = %f"), *Direction, NewLocation.X, TargetLocation.X);
	GEngine->AddOnScreenDebugMessage(999, 5.f, FColor::Yellow, DebugMessage);
}
