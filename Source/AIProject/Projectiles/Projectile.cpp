// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Projectiles/Projectile.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create DefaultSceneRoot
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    // Set up Weapon mesh
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
    ProjectileMesh->SetupAttachment(RootComponent);


    // Initialize CollisionSphere component
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->InitSphereRadius(4.0f);  // Set the sphere radius as needed
    CollisionSphere->SetupAttachment(RootComponent);

    // Initialize NiagaraSystem component
    NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
    NiagaraSystem->SetupAttachment(RootComponent);

    // Initialize ProjectileMovement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = RootComponent;
    ProjectileMovement->InitialSpeed = 5000.0f;
    ProjectileMovement->MaxSpeed = 5000.0f;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->Bounciness = 0.3f;

    // Replication settings
    SetReplicates(true);  // Enable replication for this actor
    SetReplicateMovement(true);  // Enable replication for movement

    // Set lifespan
    SetLifeSpan(10.0f);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* AProjectile::DrawRadialAtk()
{
	/// debug msg
	if (bDebugMsg && GEngine)
	//if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3,
			2.f,
			FColor::Yellow,
			FString(TEXT("Drawing Debug Sphere"))
		);
	}

	// get owning character
	AActor* owningActor = GetOwner();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3,
			2.f,
			FColor::Yellow,
			FString::Printf(TEXT("%s Owner is %s"), *this->GetName(), *owningActor->GetName())
			);
	}
	ACharacter* owningCharacter = Cast<ACharacter>(owningActor);

	// draw a line between points
	// compare current position to last position (if there is a last position saved)
	// is there a prev saved point?
	// record current position

	if (owningCharacter)
	// find the focal point
	{
		FVector FocalPoint = CollisionSphere->GetComponentLocation();

		// collision parameters - ignore self and owner
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(owningCharacter);

		//	collect variables
		FCollisionShape Sphere = FCollisionShape::MakeSphere(CollisionSphere->GetUnscaledSphereRadius());
		FHitResult HitResult;	// just a declaration, variable will be assigned a value by ref in the next function if an object is found

		///	Check for collisions via UE5 line trace function
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			LastKnownPosition,
			FocalPoint,
			ECC_Camera,
			QueryParams
		);

		///	Check for collisions via UE5 sweep function
		GetWorld()->SweepSingleByChannel(
			HitResult,
			FocalPoint,
			FocalPoint,
			FQuat::Identity,
			ECC_Pawn,
			Sphere
		);

		if (bDrawDebug) { DrawDebugSphere(GetWorld(), FocalPoint, CollisionSphere->GetUnscaledSphereRadius(), 12, HitResult.bBlockingHit ? FColor::Green : FColor::Red, true, 1.0f); }
		//if (bDrawDebug) { DrawDebugSphere(GetWorld(), FocalPoint, CollisionSphere->GetUnscaledSphereRadius(), 12, FColor::Red, false, 1.0f); }
		//if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *FocalPoint.ToCompactString()); }

	//	return HitResult.GetActor();
	}
	return nullptr;
}

AActor* AProjectile::DrawLine()
{
	/// debug msg
	if (bDebugMsg && GEngine)
		//if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3,
			2.f,
			FColor::Yellow,
			FString(TEXT("Drawing Debug Sphere"))
		);
	}

	// get owning character
	AActor* owningActor = GetOwner();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3,
			2.f,
			FColor::Yellow,
			FString::Printf(TEXT("%s Owner is %s"), *this->GetName(), *owningActor->GetName())
		);
	}
	ACharacter* owningCharacter = Cast<ACharacter>(owningActor);

	// draw a line between points
	// compare current position to last position (if there is a last position saved)
	// is there a prev saved point?
	// record current position

	if (owningCharacter)
	{
		// find the focal point
		FVector NewPosition = CollisionSphere->GetComponentLocation();

		// collision parameters - ignore self and owner
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(owningCharacter);

		//	collect variables
		//FCollisionShape Sphere = FCollisionShape::MakeSphere(CollisionSphere->GetUnscaledSphereRadius());
		FHitResult HitResult;	// just a declaration, variable will be assigned a value by ref in the next function if an object is found

		///	Check for collisions via UE5 line trace function
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			LastKnownPosition,
			NewPosition,
			ECC_Camera,
			QueryParams
		);

		/////	Check for collisions via UE5 sweep function
		//GetWorld()->SweepSingleByChannel(
		//	HitResult,
		//	FocalPoint,
		//	FocalPoint,
		//	FQuat::Identity,
		//	ECC_Pawn,
		//	Sphere
		//);

		if (bDrawDebug) { DrawDebugLine(GetWorld(), LastKnownPosition, NewPosition, HitResult.bBlockingHit ? FColor::Green : FColor::Red, true, 1.0f); }
		//if (bDrawDebug) { DrawDebugSphere(GetWorld(), NewPosition, CollisionSphere->GetUnscaledSphereRadius(), 12, HitResult.bBlockingHit ? FColor::Green : FColor::Red, true, 1.0f); }
		if (bDrawDebug) { DrawDebugSphere(GetWorld(), NewPosition, CollisionSphere->GetUnscaledSphereRadius(), 12, FColor::Red, true, 1.0f); }
		//if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *NewPosition.ToCompactString()); }

	//	return HitResult.GetActor();
	}
	return nullptr;
}
