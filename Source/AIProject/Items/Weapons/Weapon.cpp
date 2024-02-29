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

	CurrentAmmo = MaxClipSize;
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

int AWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}

void AWeapon::SetCurrentAmmo(int AmmoUsed)
{
	CurrentAmmo = CurrentAmmo - AmmoUsed;	
}

bool AWeapon::HasAmmo()
{
	return CurrentAmmo > 0;
}

int AWeapon::GetMaxClipSize()
{
	return MaxClipSize;
}

void AWeapon::Reload()
{
	// To add:
	// Number of clips available and a check to see if one is available
	// Reload key mapping
	// Animation or wait to use gun after reload
	CurrentAmmo = MaxClipSize;
}

