// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAIController.h"
#include "AIProject/Characters/AIProjectCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include "Perception/AIPerceptionComponent.h"

void ABasicAIController::BeginPlay()
{
	Super::BeginPlay();

	/// debug msg
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString(TEXT("AIController::BeginPlay()"))
		);
	}

	// start up the behaviour tree
	if (AIBehavior)
	{
		RunBehaviorTree(AIBehavior);

		// initialise blackboard values				// key					// value
		GetBlackboardComponent()->SetValueAsVector	(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool	(TEXT("IsAlive"),		true);
	}

	// bind something
	if (HasAuthority()) // is this necessary?
	{

	}

	/// debug msg
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString(TEXT("Behavior Tree Invalid"))
		);
	}
}

//ABasicAIController::ABasicAIController()
//{
//	// create AI perception component (handles perception)
//	//PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent ")); // AI perception component already in AAIController?
//}

void ABasicAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//// get the target player from the blackboard
	//UObject* TargetPlayer = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetPlayer"));
	//
	//// if it's not set find a new target
	//if (!TargetPlayer)
	//{
	//	TargetPlayer = ChooseTarget();
	//
	//	// set blackboard value						// key					// value
	//	GetBlackboardComponent()->SetValueAsObject	(TEXT("TargetPlayer"),	TargetPlayer);
	//}
	//
	//// if you have a target player
	//if (TargetPlayer != nullptr)
	//{
	//	// cast it
	//	if (APawn* PlayerPawn = Cast<APawn>(TargetPlayer))
	//	{
	//		// check they're within line of sight
	//		if (LineOfSightTo(PlayerPawn))
	//		{
	//			// save their position on blackboard		// key								// value
	//			GetBlackboardComponent()->SetValueAsVector	(TEXT("PlayerLocation"),			PlayerPawn->GetActorLocation());
	//			GetBlackboardComponent()->SetValueAsVector	(TEXT("LastKnownPlayerLocation"),	PlayerPawn->GetActorLocation());
	//
	//		}
	//		// otherwise clear the player location
	//		else
	//		{
	//			/// debug msg
	//			//if (GEngine)
	//			//{
	//			//	GEngine->AddOnScreenDebugMessage(
	//			//		-1,
	//			//		15.f,
	//			//		FColor::Yellow,
	//			//		FString(TEXT("Player not visible"))
	//			//	);
	//			//}
	//
	//			//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), );
	//			GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	//		}
	//	}
	//}
}

void ABasicAIController::UpdateBlackboard()
{
	EActionState actionState = Cast<AAIProjectCharacter>(GetPawn())->GetActionState();

	// initialise blackboard values				// key					// value
	GetBlackboardComponent()->SetValueAsEnum(TEXT("Action State"), (uint8)actionState);
}

APawn* ABasicAIController::ChooseTarget()
{
	// basic implementation, only returns player 1
	APawn* Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	// if target is within line of sight, return it
	if (LineOfSightTo(Target))	{ return Target; }
	// otherwise return null
	return nullptr;
}
