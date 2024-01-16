// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIProject/Interfaces/InteractionInterface.h"
#include "Door.generated.h"

// Forward declarations 
class USkeletalMeshComponent;
class UAnimationAsset;
class UAudioComponent;
class USoundCue;

UCLASS()
class AIPROJECT_API ADoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ADoor();

	// Implementing "Use" function from our InteractInterface
	virtual void Use(APlayerController* PlayerController) override;

	// Implementation of the interface function "GetInteractableMeshComponent"
	virtual UMeshComponent* GetInteractableMeshComponent() const override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh for the door
	UPROPERTY(EditAnywhere, Category = "Appearance")
	USkeletalMeshComponent* DoorMesh;

	// Animation to open the door
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UAnimationAsset* OpenAnimation;

	// Animation to close the door
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UAnimationAsset* CloseAnimation;

	// Sound door makes when opened
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* OpenSoundCue;

	// Sound door makes when closed
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* CloseSoundCue;

	// Boolean to check if door is open or closed (Replicated across all clients)
	UPROPERTY(ReplicatedUsing = OnRep_DoorToggled)
	bool bDoorOpen;

	// Boolean to check if door is locked (replicated across all clients)
	UPROPERTY(Replicated)
	bool bIsLocked;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_DoorToggled();

	UFUNCTION()
	void OnAnimationComplete();

private:	

	// Audio Component (Can play Sound cues)
	UPROPERTY(Transient)
	UAudioComponent* AudioComponent;

	// Boolean to check state to see if an animation is currently playing
	bool bIsAnimating;

	// Set timer handle for our animation completion
	FTimerHandle TimerHandle_AnimationComplete;

	// Animation Duration which will be set dynamically
	float AnimationDuration;

};
