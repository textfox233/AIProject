// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SpookyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API USpookyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaTime);

public:
	UPROPERTY(BlueprintReadOnly)
		class AAIProjectCharacter* AIProjectCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* AIProjectCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Speed = 0.f;

	//ECharacterState CharacterState;
};
