// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProject/Items/Weapons/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up Weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	RootComponent = WeaponMesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform AWeapon::GetMuzzleTransform()
{
	if (WeaponMesh)
	{
		if (WeaponMesh->DoesSocketExist("Muzzle"))
		{
			return WeaponMesh->GetSocketTransform("Muzzle");
		}
	}

	return FTransform::Identity;
}