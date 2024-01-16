// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIProject/Interfaces/InteractionInterface.h"
#include "Elevator.generated.h"

UCLASS()
class AIPROJECT_API AElevator : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

	// Implementing "Use" function from our InteractInterface
	virtual void Use(APlayerController* PlayerController) override;

	// Implementation of the interface function "GetInteractableMeshComponent"
	virtual UMeshComponent* GetInteractableMeshComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh for the elevator
	UPROPERTY(EditAnywhere, Category = "Appearance")
	UStaticMeshComponent* ElevatorMesh;

	// Boolean to check if elevator is up or down (Replicated across all clients)
	UPROPERTY(ReplicatedUsing = OnRep_ElevatorToggled)
	bool bElevatorUp;

	// Function to start the elevator's movement after an initial delay
	void StartMoving();

	// Speed of the elevator movement
	UPROPERTY(EditAnywhere, Category = "Elevator Movement")
	float MovementSpeed;

	// Initial delay before the elevator starts moving
	UPROPERTY(EditAnywhere, Category = "Elevator Movement")
	float InitialDelay;

	UPROPERTY(EditAnywhere, Category = "Elevator Movement")
	float UpElevation;

	UPROPERTY(EditAnywhere, Category = "Elevator Movement")
	float DownElevation;

	// Function to move the elevator up or down
	void MoveElevator(float DeltaTime);

	// Timer handle for the initial delay
	FTimerHandle TimerHandle_InitialDelay;

	// Timer handle for ending the movement
	FTimerHandle TimerHandle_Movement;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ElevatorToggled();

private:
	// Boolean to check state to see if movement is currently happening
	bool bIsMoving;
};
