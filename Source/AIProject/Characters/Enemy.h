// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "AIProjectCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class AIPROJECT_API AEnemy : public AAIProjectCharacter
{
	GENERATED_BODY()

	/** Perception Component */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	//class UAIPerceptionComponent* AIPerceptionComponent;

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:	


};
