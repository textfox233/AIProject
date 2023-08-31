// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Wander.h"

EBTNodeResult::Type UBTT_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/// debug msg
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Yellow,
	//		FString(TEXT("Wander Task Executing"))
	//	);
	//}

	return EBTNodeResult::Succeeded;
}