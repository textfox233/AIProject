// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class AIPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to get the transform of the muzzle socket
	FTransform GetMuzzleTransform();

	UFUNCTION(BlueprintPure)
	int GetCurrentAmmo();

	void SetCurrentAmmo(int AmmoUsed);

	UFUNCTION(BlueprintPure)
	bool HasAmmo();

	UFUNCTION(BlueprintPure)
	int GetMaxClipSize();

	void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Appearance")
	USkeletalMeshComponent* WeaponMesh;

private:

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int MaxClipSize = 10;

	// Set to VisibleAnywhere after testing
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int CurrentAmmo;

};
