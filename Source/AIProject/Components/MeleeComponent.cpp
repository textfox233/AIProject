// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeComponent.h"
//#include "AIProject/Characters/AIProjectCharacter.h"

#include "GameFramework/Character.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeleeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	/// debug msg
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString(TEXT("UMeleeComponent::BeginPlay()"))
		);
	}
}


// Called every frame
void UMeleeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/// -- Set linetraces to occur until TriggerMeleeEnd()
// Start traces
void UMeleeComponent::MeleeTraceStart()
{
	// start timer
	GetWorld()->GetTimerManager().SetTimer(MeleeTraceHandle, this, &UMeleeComponent::MeleeTraceInProgress, 0.01f, true, 0.05f);
}
// Perform single trace
void UMeleeComponent::MeleeTraceInProgress()
{
	//UE_LOG(LogTemp, Warning, TEXT("timer active"));

	// perform line trace (debug? / logs?)
	AActor* hit = DrawRadialAtk(true, false);

	//// process the hit
	//if (UMeleeComponent(hit))
	//	// if hit was valid
	//{
	//	// stop line tracing - should stop multiple hits per swing
	//	GetWorld()->GetTimerManager().ClearTimer(MeleeTraceHandle);
	//}

	//if (hit != nullptr)
	//// anything hit?
	//{
	//	// process hit
	//	if (ProcessMeleeHit(hit))
	//	// if hit was valid
	//	{
	//
	//	}
	//}
}
// End traces
void UMeleeComponent::MeleeTraceEnd()
{
	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(MeleeTraceHandle);
}

// -- Draw a line trace to track a weapon's movement and detect hit events
AActor* UMeleeComponent::DrawRadialAtk(bool bDrawDebug, bool bDebugLog)
{
	/// debug msg
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Yellow,
	//		FString(TEXT("Drawing Debug Sphere"))
	//	);
	//}
	 
	// define points for line trace
	//FHitResult Hit;

	// get owner
	AActor* characterOwner = GetOwner();

	// cast it
	ACharacter* targetCharacter = Cast<ACharacter>(characterOwner);

	if (targetCharacter)
	// find the focal point
	FVector focalPoint;
	{
		// get bone
		FName leftHandBone = "hand_l";
		FVector focalPoint = targetCharacter->GetMesh()->GetBoneLocation(leftHandBone, EBoneSpaces::WorldSpace);
		if (bDrawDebug) { DrawDebugSphere(GetWorld(), focalPoint, 10.f, 12, FColor::Red, false, 1.0f); }
		if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *focalPoint.ToCompactString()); }
	}

	// grab focal point
	//const USkeletalMeshComponent* skComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	//USkeletalMesh* skMesh = skComp->GetSkeletalMeshAsset();
	//USkeletalMesh* skMesh = EquippedWeapon->FindComponentByClass<USkeletalMesh>();

	// grab sockets
	//FVector focalPoint = skMesh->GetSocketByIndex(0)->GetSocketTransform(skComp).GetLocation();
	//FVector focalPoint = characterOwner->

	//// collision parameters - ignore self
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(this);
	
	// perform line trace
	//GetWorld()->LineTraceSingleByChannel(Hit, traceStart, traceEnd, ECollisionChannel::ECC_Camera, QueryParams);

	// Debug
	//if (bDrawDebug) { DrawDebugSphere(GetWorld(), focalPoint, 50.f, 12, FColor::Red); }
	//if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *focalPoint.ToCompactString()); }

	/// if hit occurs and hit actor is valid
	//if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	//{
	//	if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName()); }
	//	// return hit actor
	//	return Hit.GetActor();
	//}

	// nothing hit
	if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("No actors hit")); }
	
	return nullptr;
}

void UMeleeComponent::BasicAtk()
{
	// cast to character
	if (ACharacter* owningCharacter = Cast<ACharacter>(GetOwner()))
	{
		// play montage
		if (BasicAttackMontage)
		{
			owningCharacter->PlayAnimMontage(BasicAttackMontage);
		}
	}
}
