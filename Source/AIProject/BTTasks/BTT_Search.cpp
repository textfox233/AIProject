// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Search.h"

EBTNodeResult::Type UBTT_Search::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/// debug msg
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			FString(TEXT("Search Task Executing"))
		);
	}

	return EBTNodeResult::Succeeded;
}