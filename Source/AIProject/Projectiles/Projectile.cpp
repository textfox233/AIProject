// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Projectiles/Projectile.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

