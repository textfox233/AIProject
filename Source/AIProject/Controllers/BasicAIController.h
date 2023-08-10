// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicAIController.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API ABasicAIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	APawn* ChooseTarget();
};
