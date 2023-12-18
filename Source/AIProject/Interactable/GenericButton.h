// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIProject/Interfaces/InteractionInterface.h"
#include "GenericButton.generated.h"

// Forward declarations 
class UMeshComponent;
class UAnimationAsset;
class UAudioComponent;
class USoundCue;
class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class AIPROJECT_API AGenericButton : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericButton();

	// Implementing "Use" function from our InteractInterface
	virtual void Use(APlayerController* PlayerController) override;

	// Implementation of the interface function "GetInteractableMeshComponent"
	virtual UMeshComponent* GetInteractableMeshComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root component to hold other components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

	// Static Mesh for the button
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UStaticMeshComponent* StaticButtonMesh;

	// Skeletal Mesh for the button
	UPROPERTY(EditAnywhere, Category = "Appearance")
	USkeletalMeshComponent* SkeletalButtonMesh;

	// Animation when button is pressed
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UAnimationAsset* TriggeredAnimation;

	// Sound when button is pressed
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* TriggeredSoundCue;

	// Boolean to check if button is locked (Replicated across all clients)
	UPROPERTY(ReplicatedUsing = OnRep_ButtonTriggered)
	bool bIsLocked;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ButtonTriggered();

	UFUNCTION()
	void OnAnimationComplete();

	UPROPERTY(Replicated)
	APlayerController* ReplicatedPlayerController;

	// Target that the button will trigger (Can be a door or any other object that the player can interact with)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;

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

	UPROPERTY(EditAnywhere, Category = "Appearance")
	bool bIsSkeletalMesh;
};
