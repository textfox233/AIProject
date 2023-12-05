// Fill out your copyright notice in the Description page of Project Settings.

#include "AIProject/Interactable/PlayerFlashlight.h"
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APlayerFlashlight::APlayerFlashlight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up Flashlight mesh
	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMeshComponent"));
	RootComponent = FlashlightMesh;

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotlightComponent"));
	SpotLightComponent->SetupAttachment(RootComponent);

	// Set collision to no collision for the flashlight mesh
	FlashlightMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// Set replication to true
	bReplicates = true;

	// Set default flashlight state
	bLightOn = false;

	// Set SpotLightComponent default visibility based on bLightOn
	SpotLightComponent->SetHiddenInGame(bLightOn);
}

// Called when the game starts or when spawned
void APlayerFlashlight::BeginPlay()
{
	Super::BeginPlay();
}

// Replication callback when the replicated light state changes
void APlayerFlashlight::OnRep_LightToggled()
{
	// Toggle visibility of SpotLightComponent based on inverse value of bLightOn boolean
	SpotLightComponent->SetHiddenInGame(!bLightOn);
}

// Specify which properties are replicated
void APlayerFlashlight::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the bLightOn variable.
	DOREPLIFETIME(APlayerFlashlight, bLightOn);
}

// Validate function for server-side flashlight toggle
bool APlayerFlashlight::Server_ToggleFlashlight_Validate()
{
	return true;
}

// Server-side function to toggle flashlight (replicated)
void APlayerFlashlight::Server_ToggleFlashlight_Implementation()
{
	ToggleFlashlight();
}

// Function to toggle the flashlight's state
void APlayerFlashlight::ToggleFlashlight()
{
	// Check if code is being executed on server
	if (HasAuthority())
	{
		SpotLightComponent->SetHiddenInGame(bLightOn);
		bLightOn = !bLightOn;
	}
	else
	{
		SpotLightComponent->SetHiddenInGame(bLightOn);
		// Call server to toggle flashlight
		Server_ToggleFlashlight();
	}
}
