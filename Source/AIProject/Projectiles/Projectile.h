// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class AIPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void SetLastKnownPosition(FVector Position)	{ LastKnownPosition = Position; }
	UFUNCTION(BlueprintCallable)
	FVector GetLastKnownPosition() { return LastKnownPosition; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root component to hold other components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* NiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

private:	
	UFUNCTION(BlueprintCallable)
	AActor* HitDetectionSpherical();

	UFUNCTION(BlueprintCallable)
	AActor* HitDetectionLinear();

	UFUNCTION(BlueprintCallable)
	bool ProcessHitActor(AActor* HitActor);

	// -- Debug booleans
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Projectile", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Projectile", meta = (AllowPrivateAccess = "true"))
	bool bPersistentShapes = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Projectile", meta = (AllowPrivateAccess = "true"))
	bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Projectile", meta = (AllowPrivateAccess = "true"))
	bool bDebugMsg = false;

	FVector LastKnownPosition;
};
