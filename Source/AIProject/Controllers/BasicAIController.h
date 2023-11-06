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
	//ABasicAIController();

	virtual void Tick(float DeltaSeconds) override;
	
	// when a player or other enemy is spotted
	//virtual void OnTargetSpotted();

	void UpdateBlackboard();

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	//	class UAIPerceptionComponent* PerceptionComponent;

	APawn* ChooseTarget();
};
