// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerFlashlight.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class AIPROJECT_API APlayerFlashlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerFlashlight();

	// Switch flashlight on and off
	void ToggleFlashlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ToggleFlashlight();

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	UStaticMeshComponent* FlashlightMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Appearance")
	USpotLightComponent* SpotLightComponent;

	UPROPERTY(ReplicatedUsing = OnRep_LightToggled)
	bool bLightOn;

	UFUNCTION()
	void OnRep_LightToggled();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:

};
