// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Interactable/GenericButton.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AGenericButton::AGenericButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create DefaultSceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Initialise properties
	TriggeredAnimation = nullptr;
	TriggeredSoundCue = nullptr;
	bIsLocked = false;
	bIsAnimating = false;

	// Set replication to true
	bReplicates = true;

	// Create AudioComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Skeletal button mesh
	SkeletalButtonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalButtonMesh->SetupAttachment(RootComponent);

	// Static Mesh component
	StaticButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticButtonMesh->SetupAttachment(RootComponent);

	// Set the button collision to "BlockAll"
	SkeletalButtonMesh->SetCollisionProfileName("BlockAll");
	StaticButtonMesh->SetCollisionProfileName("BlockAll");

	// Set default value for Render Custom Depth to false - Used to highlight the object you look at
	SkeletalButtonMesh->SetRenderCustomDepth(false);
	StaticButtonMesh->SetRenderCustomDepth(false);
}

void AGenericButton::Use(APlayerController* PlayerController)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Called DoorButton"));

	if (HasAuthority() && !bIsAnimating && !bIsLocked)
	{

		// Set the flag to indicate animation is starting
		bIsAnimating = true;

		// Set the replicated PlayerController variable
		ReplicatedPlayerController = PlayerController;

		// Execute on all clients (Replication)
		OnRep_ButtonTriggered();
	}
}

UMeshComponent* AGenericButton::GetInteractableMeshComponent() const
{
	return bIsSkeletalMesh ? Cast<UMeshComponent>(SkeletalButtonMesh) : Cast<UMeshComponent>(StaticButtonMesh);
}

// Called when the game starts or when spawned
void AGenericButton::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericButton::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGenericButton, bIsLocked);
}

void AGenericButton::OnRep_ButtonTriggered()
{
	if (TriggeredAnimation)
	{
		AnimationDuration = TriggeredAnimation->GetPlayLength();
		SkeletalButtonMesh->PlayAnimation(TriggeredAnimation, false);

		// Set a timer to reset the bIsAnimating flag after animation duration
		GetWorldTimerManager().SetTimer(TimerHandle_AnimationComplete, this, &AGenericButton::OnAnimationComplete, AnimationDuration, false);
	}
	else
	{
		// Wait for a certain duration (adjust WaitDurationInSeconds as needed)
		const float WaitDurationInSeconds = 1.0f; // Adjust this value based on your requirement
		GetWorldTimerManager().SetTimer(TimerHandle_AnimationComplete, this, &AGenericButton::OnAnimationComplete, WaitDurationInSeconds, false);
	}

	if (TriggeredSoundCue)
	{
		FVector SoundLocation = GetActorLocation();
		AudioComponent->SetSound(TriggeredSoundCue);
		AudioComponent->SetWorldLocationAndRotation(SoundLocation, GetActorRotation());
		AudioComponent->Play();
	}

	if (TargetActor)
	{
		if (AActor* Actor = TargetActor)
		{
			if (HasAuthority())
			{
				if (IInteractionInterface* Interface = Cast<IInteractionInterface>(Actor))
				{
					APlayerController* PlayerController = ReplicatedPlayerController;
					Interface->Use(PlayerController);
				}
			}
		}
	}

}

void AGenericButton::OnAnimationComplete()
{
	// Reset the flag after animation is done
	bIsAnimating = false;
}