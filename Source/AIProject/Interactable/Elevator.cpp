// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Interactable/Elevator.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up elevator mesh
	ElevatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// Set root component to our elevator mesh
	RootComponent = ElevatorMesh;

	// Set the elevator collision to "BlockAll"
	ElevatorMesh->SetCollisionProfileName("BlockAll");

	// Set default value for Render Custom Depth to false - Used to highlight the object you look at
	ElevatorMesh->SetRenderCustomDepth(false);

	// Ensure elevator is down to begin with
	bElevatorUp = false;

	// Set replication to true
	bReplicates = true;

	// Set movement parameters
	bIsMoving = false;
	MovementSpeed = 100.0f;
	InitialDelay = 2.0f;
	DownElevation = 0.0f;
	UpElevation = 300.0f;
}

void AElevator::Use(APlayerController* PlayerController)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Called ElevatorToggled"));

	if (HasAuthority() && !bIsMoving)
	{

		// Set bElevatorUp to the opposite value (toggle true or false)
		bElevatorUp = !bElevatorUp;

		// Execute ElevatorToggled on all clients (Replication)
		OnRep_ElevatorToggled();

		// Execute StartMoving on the server (Replication)
		//Server_StartMoving();
		//GetWorldTimerManager().SetTimer(TimerHandle_InitialDelay, this, &AElevator::Server_StartMoving, InitialDelay, false);
	}
}

UMeshComponent* AElevator::GetInteractableMeshComponent() const
{
	// Return the mesh component of this actor
	return ElevatorMesh;
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		MoveElevator(DeltaTime);
	}
}

void AElevator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AElevator, bElevatorUp);
}

void AElevator::OnRep_ElevatorToggled()
{
	// Start the initial delay timer to begin movement after a delay
	GetWorldTimerManager().SetTimer(TimerHandle_InitialDelay, this, &AElevator::StartMoving, InitialDelay, false);
}


void AElevator::StartMoving()
{
	// Initial delay has passed, start the movement
	bIsMoving = true;
	GetWorldTimerManager().ClearTimer(TimerHandle_InitialDelay);
	OnRep_ElevatorToggled();
}

void AElevator::MoveElevator(float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "ELEVATOR CALLED AFTER DELAY");

	if (bElevatorUp)
	{
		// Move the elevator up
		NewLocation = CurrentLocation + FVector(0, 0, MovementSpeed * DeltaTime);

		if (NewLocation.Z >= UpElevation)
		{
			NewLocation.Z = UpElevation; // Ensure the elevator stops at the target elevation
			bIsMoving = false;
		}

	}
	else
	{
		// Move the elevator down
		NewLocation = CurrentLocation - FVector(0, 0, MovementSpeed * DeltaTime);

		if (NewLocation.Z <= DownElevation)
		{
			NewLocation.Z = DownElevation; // Ensure the elevator stops at the target elevation
			bIsMoving = false;
		}
	}

	SetActorLocation(NewLocation);
}