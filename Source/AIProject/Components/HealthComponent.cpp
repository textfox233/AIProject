// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = MaxHealth;
}

void UHealthComponent::TakeDamage(float damageAmount)
{
	if (bDebugMsg && GEngine)
	{
		FString msg{ GetOwner()->GetName() + TEXT(" took damage!") };
		GEngine->AddOnScreenDebugMessage(
			2,
			1.f,
			FColor::Cyan,
			msg
		);
	}

	float prevHealth = Health;
	Health -= damageAmount;

	if (bDebugMsg && GEngine)
	{
		FString msg = FString::Printf(TEXT("Health was %f, is now %f!"), prevHealth, Health);
		GEngine->AddOnScreenDebugMessage(
			2,
			1.f,
			FColor::Cyan,
			msg
		);
	}

}



// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	/// debug msg
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Green,
	//		FString(TEXT("UHealthComponent::BeginPlay()"))
	//		);
	//}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

