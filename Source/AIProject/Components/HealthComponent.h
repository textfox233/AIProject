// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	// -- Health Values
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float Health;

	// -- Debug booleans
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Health Component", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Health Component", meta = (AllowPrivateAccess = "true"))
	bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Health Component", meta = (AllowPrivateAccess = "true"))
	bool bDebugMsg = false;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	bool TakeDamage(float damageAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
