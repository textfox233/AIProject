// Fill out your copyright notice in the Description page of Project Settings.


#include "SpookyAnimInstance.h"
#include "AIProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USpookyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Get character ref
	AIProjectCharacter = Cast<AAIProjectCharacter>(TryGetPawnOwner());

	// Get movement component ref
	if (AIProjectCharacter)
	{
		AIProjectCharacterMovement = AIProjectCharacter->GetCharacterMovement();
	}
}

void USpookyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (AIProjectCharacterMovement)
	{
		// Get Variables
		Speed = UKismetMathLibrary::VSizeXY(AIProjectCharacterMovement->Velocity);
		//CharacterState = SirDingusCharacter->GetCharacterState();
	}
}