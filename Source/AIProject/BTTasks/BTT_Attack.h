// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class AIPROJECT_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
	/** Debug booleans */
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebugMsg = false;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
