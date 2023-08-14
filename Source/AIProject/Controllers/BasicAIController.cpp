// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABasicAIController::BeginPlay()
{
	Super::BeginPlay();

	// start up the behaviour tree
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);

		// initialise blackboard values				// key					// value
		GetBlackboardComponent()->SetValueAsVector	(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool	(TEXT("IsAlive"),		true);
	}

	// debug msg
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Behavior Tree Invalid"))
		);
	}
}

void ABasicAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// get the target player from the blackboard
	UObject* TargetPlayer = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayer"));

	// if it's not set find a new target
	if (!TargetPlayer)
	{
		TargetPlayer = ChooseTarget();

		// set blackboard value						// key					// value
		GetBlackboardComponent()->SetValueAsObject	(TEXT("TargetPlayer"),	TargetPlayer);
	}

	// if you have a target player
	if (TargetPlayer != nullptr)
	{
		// cast it
		if (APawn* PlayerPawn = Cast<APawn>(TargetPlayer))
		{
			// check they're within line of sight
			if (LineOfSightTo(PlayerPawn))
			{
				// save their position on blackboard		// key								// value
				GetBlackboardComponent()->SetValueAsVector	(TEXT("PlayerLocation"),			PlayerPawn->GetActorLocation());
				GetBlackboardComponent()->SetValueAsVector	(TEXT("LastKnownPlayerLocation"),	PlayerPawn->GetActorLocation());

			}
		}
	}
}

APawn* ABasicAIController::ChooseTarget()
{
	// basic implementation, only returns player 1
	APawn* Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	// if target is within line of sight, return it
	

	return nullptr;
}
