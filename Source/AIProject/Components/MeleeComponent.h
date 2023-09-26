// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPROJECT_API UMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeleeComponent();

	/// -- Set linetraces to occur until TriggerMeleeEnd()
	// Start traces
	void MeleeTraceStart();
	// Perform single trace
	void MeleeTraceInProgress();
	// End traces
	void MeleeTraceEnd();

	// -- Draw a line trace to track a weapon's movement and detect hit events
	AActor* DrawRadialAtk();

	void PerformBasicAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// -- Timer Handle
	FTimerHandle MeleeTraceHandle;

	/** Animation Montages **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess))
		class UAnimMontage* BasicAttackMontage;

private:
	// -- Debug booleans
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Debug, meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Debug, meta = (AllowPrivateAccess = "true"))
		bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Debug, meta = (AllowPrivateAccess = "true"))
		bool bDebugMsg = false;
};
