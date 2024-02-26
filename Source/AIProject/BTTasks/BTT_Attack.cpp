// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"

#include "AIProject/Characters/AIProjectCharacter.h"
#include "AIProject/Components/MeleeComponent.h"

#include "AIController.h"


EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Display, TEXT("AI Attacking"));
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString(TEXT("Attack Task Executing"))
		);
	}

	// get melee component
	if (AAIProjectCharacter* ownerCharacter = Cast<AAIProjectCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (UMeleeComponent* meleeComponent = ownerCharacter->MeleeComponent)
		{
			// perform basic attack using melee component
			meleeComponent->PerformBasicAttack();
		}
		else if (bDebugMsg && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Red,
				FString(TEXT("No Melee Component"))
			);
		}
	}
	else if (bDebugMsg && GEngine)
	{
		FString debugMessage = FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetAIOwner()->GetPawn()->GetName());
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			//FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetOwner()->GetName())
			debugMessage
		);
	}



	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}
