// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerFlashlight.generated.h"

// Forward declarations to reduce compilation dependencies
class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class AIPROJECT_API APlayerFlashlight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerFlashlight();

	// Function to switch flashlight on and off
	void ToggleFlashlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Server-side function to toggle flashlight (replicated)
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ToggleFlashlight();

	// Mesh component for the flashlight
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UStaticMeshComponent* FlashlightMesh;

	// SpotLight component for the flashlight
	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	USpotLightComponent* SpotLightComponent;

	// Replicated boolean indicating whether the light is on or off
	UPROPERTY(ReplicatedUsing = OnRep_LightToggled)
	bool bLightOn;

	// Function called when the replicated light state changes
	UFUNCTION()
	void OnRep_LightToggled();

	// Function to specify which properties are replicated
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	// Private members or functions (if any)
};
