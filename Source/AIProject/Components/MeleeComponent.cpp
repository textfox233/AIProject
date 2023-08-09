// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeComponent.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Green,
			FString(TEXT("UMeleeComponent::BeginPlay()"))
		);
	}
}


// Called every frame
void UMeleeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

