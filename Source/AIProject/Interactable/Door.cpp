// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Interactable/Door.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up door mesh
	DoorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	// Set root component to our door mesh
	RootComponent = DoorMesh;

	// Set the door collision to "BlockAll"
	DoorMesh->SetCollisionProfileName("BlockAll");

	// Set default value for Render Custom Depth to false - Used to highlight the object you look at
	DoorMesh->SetRenderCustomDepth(false);

	// Set up audio component to play open & close sounds
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	// Attach to root component
	AudioComponent->SetupAttachment(RootComponent);

	// Ensure door is closed to begin with
	bDoorOpen = false;

	// Set replication to true
	bReplicates = true;

	// Set initial animation state to false
	bIsAnimating = false;

	// Set initial lock state
	bIsLocked = false;

}

void ADoor::Use(APlayerController* PlayerController)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Called ToggleDoor"));

	if (HasAuthority() && !bIsAnimating && !bIsLocked)
	{

		// Set the flag to indicate animation is starting
		bIsAnimating = true;

		// Set bDoorOpen to the opposite value (toggle true or false)
		bDoorOpen = !bDoorOpen;

		// Execute DoorToggled on all clients (Replication)
		OnRep_DoorToggled();
	}
}

UMeshComponent* ADoor::GetInteractableMeshComponent() const
{
	// Return the mesh component of this actor
	return DoorMesh;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bDoorOpen);
	DOREPLIFETIME(ADoor, bIsLocked);
}

void ADoor::OnRep_DoorToggled()
{
	FVector SoundLocation = GetActorLocation();

	if (bDoorOpen)
	{
		if (OpenAnimation)
		{
			AnimationDuration = OpenAnimation->GetPlayLength();
			DoorMesh->PlayAnimation(OpenAnimation, false);
			if (OpenSoundCue)
			{
				AudioComponent->SetSound(OpenSoundCue);
				AudioComponent->SetWorldLocationAndRotation(SoundLocation, GetActorRotation());
				AudioComponent->Play();
			}
		}
	}
	else
	{
		if (CloseAnimation)
		{
			AnimationDuration = CloseAnimation->GetPlayLength();
			DoorMesh->PlayAnimation(CloseAnimation, false);
			if (CloseSoundCue)
			{
				AudioComponent->SetSound(CloseSoundCue);
				AudioComponent->SetWorldLocationAndRotation(SoundLocation, GetActorRotation());
				AudioComponent->Play();
			}
		}
	}

	// Set a timer to reset the bIsAnimating flag after animation duration
	GetWorldTimerManager().SetTimer(TimerHandle_AnimationComplete, this, &ADoor::OnAnimationComplete, AnimationDuration, false);

}

void ADoor::OnAnimationComplete()
{
	// Reset the flag after animation is done
	bIsAnimating = false;
}