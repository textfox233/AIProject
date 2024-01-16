// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIProject/Interfaces/InteractionInterface.h"
#include "SlidingDoor.generated.h"

class UArrowComponent;

UCLASS()
class AIPROJECT_API ASlidingDoor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingDoor();

	// Implementing "Use" function from our InteractInterface
	virtual void Use(APlayerController* PlayerController) override;

	// Implementation of the interface function "GetInteractableMeshComponent"
	virtual UMeshComponent* GetInteractableMeshComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root component to hold other components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root", meta = (AllowPrivateAccess = "true"))

	USceneComponent* DefaultSceneRoot;
	// Arrow component to visualize the forward vector
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ForwardArrowComponent;

	// Mesh for the door
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UStaticMeshComponent* DoorMesh;

	// Boolean to check if door is open or closed (Replicated across all clients)
	UPROPERTY(ReplicatedUsing = OnRep_DoorToggled)
	bool bDoorOpened;

	// Function to start the door's movement after an initial delay
	void StartMoving();

	// Speed of the door movement
	UPROPERTY(EditAnywhere, Category = "Door Movement")
	float MovementSpeed;

	// Initial delay before the door starts moving
	UPROPERTY(EditAnywhere, Category = "Door Movement")
	float InitialDelay;

	UPROPERTY(EditAnywhere, Category = "Door Movement")
	float DoorTargetX;

	// Function to move the door left or right
	void MoveDoor(float DeltaTime);

	// Timer handle for the initial delay
	FTimerHandle TimerHandle_InitialDelay;

	// Timer handle for ending the movement
	FTimerHandle TimerHandle_Movement;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_DoorToggled();

private:

	// Boolean to check state to see if movement is currently happening
	bool bIsMoving;

	FVector TargetLocation;
};
