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

	void PerformBasicAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/// -- Set sphere draws to occur until EndHitDetection()
	// Start drawing
	UFUNCTION(BlueprintCallable)
		void StartHitDetection();
	// Perform and evaluate a single draw
	void HitDetectionInProgress();
	// Stop Drawing
	UFUNCTION(BlueprintCallable)
		void EndHitDetection();

	// -- Draw a sphere to detect hit events
	AActor* DrawRadialAtk(FVector originPoint);

	// -- Draw a line to detect hit events
	AActor* DetectLinearAtk(FVector startPoint, FVector endPoint);

	// -- Determine if anything was hit by a sphere, and if damage should be applied. Returns TRUE if any damage was dealt
	bool ProcessMeleeHit(AActor* hitActor);


public:	
	/** Animation Montages **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess))
		class UAnimMontage* BasicAttackMontage;

private:
	// -- Debug booleans
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|MeleeComponent", meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|MeleeComponent", meta = (AllowPrivateAccess = "true"))
		bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|MeleeComponent", meta = (AllowPrivateAccess = "true"))
		bool bDebugMsg = false;

		//UPROPERTY(Category = "Collision")
		//UPROPERTY(EditDefaultsOnly)
	//UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
	//	FComponentReference RightUnarmed;

	//UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
	//	USceneComponent* SceneComponent;


	//	UActorComponent* RightUnarmedCollisionComponent;

	// -- Timer Handle
	FTimerHandle TimerHandle;
};