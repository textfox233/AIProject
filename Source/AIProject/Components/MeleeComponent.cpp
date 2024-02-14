// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeComponent.h"
#include "AIProject/Characters/AIProjectCharacter.h"

#include "Kismet/GameplayStatics.h"
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
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Cyan,
			FString(TEXT("UMeleeComponent::BeginPlay()"))
		);
	}
}


/// -- Set linetraces to occur until TriggerMeleeEnd()
// Start traces
void UMeleeComponent::StartHitDetection()
{
	// start timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UMeleeComponent::HitDetectionInProgress, 0.01f, true, 0.05f);

	if (bDebugLog) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Detection Starting"));
	}
}
// Perform a single sphere draw
void UMeleeComponent::HitDetectionInProgress()
{
	//UE_LOG(LogTemp, Warning, TEXT("timer active"));

	// perform line trace
	AActor* hit = DrawRadialAtk();

	/// process the hit
	//if (UMeleeComponent(hit))
	//	// if hit was valid
	//{
	//	// stop line tracing - should prevent multiple hits per swing
	//	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//}

	// process the hit
	if (ProcessMeleeHit(hit))
		// if hit was valid
	{
		// stop line tracing - should stop multiple hits per swing
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

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
void UMeleeComponent::EndHitDetection()
{
	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	if (bDebugLog) {
		UE_LOG(LogTemp, Warning, TEXT("Hit Detection Ending"));
	}
}

// -- Draw a line trace to track a weapon's movement and detect hit events
AActor* UMeleeComponent::DrawRadialAtk()
{
	/// debug msg
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			3,
			2.f,
			FColor::Yellow,
			FString(TEXT("Drawing Debug Sphere"))
		);
	}
	 
	// get owning character
	AActor* characterOwner = GetOwner();
	ACharacter* targetCharacter = Cast<ACharacter>(characterOwner);

	if (targetCharacter)
	// find the focal point
	{
		// get bone
		FName leftHandBone = "hand_l";
		FVector focalPoint = targetCharacter->GetMesh()->GetBoneLocation(leftHandBone, EBoneSpaces::WorldSpace);

		// collision parameters - ignore self
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(characterOwner);

		//	collect variables
		FCollisionShape Sphere = FCollisionShape::MakeSphere(10.f);
		FHitResult HitResult;	// just a declaration, variable will be assigned a value by ref in the next function if an object is found

		///	Check for collisions via UE5 sweep function
		GetWorld()->SweepSingleByChannel(
			HitResult,
			focalPoint,
			focalPoint,
			FQuat::Identity,
			ECC_Pawn,
			Sphere
		);

		if (bDrawDebug) { DrawDebugSphere(GetWorld(), focalPoint, 10.f, 12, HitResult.bBlockingHit ? FColor::Green : FColor::Red, false, 1.0f); }
		if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *focalPoint.ToCompactString()); }

		return HitResult.GetActor();
	}

	// grab focal point
	//const USkeletalMeshComponent* skComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	//USkeletalMesh* skMesh = skComp->GetSkeletalMeshAsset();
	//USkeletalMesh* skMesh = EquippedWeapon->FindComponentByClass<USkeletalMesh>();
	//
	// grab sockets
	//FVector focalPoint = skMesh->GetSocketByIndex(0)->GetSocketTransform(skComp).GetLocation();
	//FVector focalPoint = characterOwner->
	//
	//
	// perform line trace
	//GetWorld()->LineTraceSingleByChannel(Hit, traceStart, traceEnd, ECollisionChannel::ECC_Camera, QueryParams);
	//
	// Debug
	//if (bDrawDebug) { DrawDebugSphere(GetWorld(), focalPoint, 50.f, 12, FColor::Red); }
	//if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Tracing sphere around %s "), *focalPoint.ToCompactString()); }
	//
	// if hit occurs and hit actor is valid
	//if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	//{
	//	if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName()); }
	//	// return hit actor
	//	return Hit.GetActor();
	//}

	/// nothing hit
	if (bDebugLog) { UE_LOG(LogTemp, Warning, TEXT("Owner isn't a character")); }
	
	return nullptr;
}

// -- Process melee hits (TRUE means damage was dealt, FALSE means the hit was invalid)
bool UMeleeComponent::ProcessMeleeHit(AActor* hitActor)
{
	// 1. Was anything hit at all?
	if (hitActor == nullptr)
	{
		if (bDebugLog) {
			UE_LOG(LogTemp, Warning, TEXT("Nothing was Hit"));
		}
		// FALSE: no target
		return false;
	}
	if (bDebugLog) {
		UE_LOG(LogTemp, Log, TEXT("ASirDingusCharacter::ProcessMeleeHit"));
		UE_LOG(LogTemp, Warning, TEXT("%s Hit"), *hitActor->GetName());
	}

	// 2. Was the hit actor an already dead character's capsule?
	if (AAIProjectCharacter* Character = Cast<AAIProjectCharacter>(hitActor))
	{
		// if character is not alive
		EActionState CharacterState = Character->GetActionState();
		if (CharacterState == EActionState::EAS_Dead)
		{
			/// Debug
			if (bDebugLog && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					2.f,
					FColor::Yellow,
					FString(TEXT("Target is already dead"))
				);
			}
			// FALSE: target invalid
			return false;
		}
	}

	/// 3. Did a player just hit another player? - (potentially legacy, may want friendly fire for this project)
	//if (GetOwner()->ActorHasTag("Player"))
	//{
	//	// check tags to see what is being damaged
	//	if (hitActor->ActorHasTag("Player"))
	//	{
	//		/// Debug
	//		if (bDebugLog)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("Target is a player"));
	//
	//			if (GEngine)
	//			{
	//				GEngine->AddOnScreenDebugMessage(
	//					-1,
	//					15.f,
	//					FColor::Yellow,
	//					FString(TEXT("Hit target is player"))
	//				);
	//			}
	//		}
	//
	//		// FALSE: target invalid
	//		return false;
	//	}
	//}
	//
	//if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("Target not a player")); }

	// Upon passing the above, deal damage to the hit actor
	if (AController* OwningController = GetOwner()->GetInstigatorController())
	{
		// deal damage to hit actors
		UClass* DamageTypeClass = UDamageType::StaticClass();
		float dmgDealt = UGameplayStatics::ApplyDamage(
			hitActor,			// DamagedActor - Actor that will be damaged.
			50,					// BaseDamage - The base damage to apply.
			OwningController,	// EventInstigator - Controller that was responsible for causing this damage (e.g. player who swung the weapon)
			GetOwner(),			// DamageCauser - Actor that actually caused the damage (e.g. the grenade that exploded)
			DamageTypeClass		// DamageTypeClass - Class that describes the damage that was done.
		);
		if (bDebugLog) { UE_LOG(LogTemp, Log, TEXT("damage dealt: %f"), dmgDealt); }
		if (bDebugMsg && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Yellow,
				FString::Printf(TEXT("ASirDingusCharacter::ProcessMeleeHit -> damage dealt: %f"), dmgDealt)
			);
		}

		// TRUE: damage was dealt
		return true;
	}
	/// Debug Message
	else
	{
		if (bDebugMsg && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Owning Controller is Invalid"))
			);
		}
		return false;
	}
}

void UMeleeComponent::PerformBasicAttack()
{
	if (bDebugMsg && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			2,
			1.f,
			FColor::Green,
			FString(TEXT("UMeleeComponent::PerformBasicAttack()"))
		);
	}

	// if NOT in unoccupied state THEN exit function and give a debug message
	// 1. Cast to custom character class
	if (AAIProjectCharacter* owningCharacter = Cast<AAIProjectCharacter>(GetOwner()))
	{
		// 2. Check state
		if (owningCharacter->GetActionState() != EActionState::EAS_Unoccupied)
		{
			/// debug msg
			if (bDebugMsg && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					2,
					1.f,
					FColor::Red,
					FString(TEXT("Owning Character is NOT unoccupied"))
				);
			}
			return;
		}
		// play the attack (and set the action state?)
		if (BasicAttackMontage)
		{
			owningCharacter->PlayAnimMontage(BasicAttackMontage);

			//owningCharacter->SetActionState(EActionState::EAS_Attacking);

			//if (bDebugMsg && GEngine)
			//{
			//	FString msg = UEnum::GetValueAsString(owningCharacter->GetActionState());
			//	//FString debugMessage = FString::Printf(TEXT("Current action state is: %s"), actionState);
			//	GEngine->AddOnScreenDebugMessage(
			//		2,
			//		5.f,
			//		FColor::Green,
			//		//FString::Printf(TEXT("Cast Failed: owner character is %s"), *OwnerComp.GetOwner()->GetName())
			//		msg
			//	);
			//}
		}
		// debug msg
		else if (bDebugMsg && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("BasicAttackMontage is Null"))
			);
		}
	}

	//// cast to base character class
	//if (ACharacter* owningCharacter = Cast<ACharacter>(GetOwner()))
	//{
	//	// play montage
	//	if (BasicAttackMontage)
	//	{
	//		owningCharacter->PlayAnimMontage(BasicAttackMontage);
	//	}
	//	// debug msg
	//	else if (bDebugMsg && GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(
	//			-1,
	//			15.f,
	//			FColor::Red,
	//			FString(TEXT("BasicAttackMontage is Null"))
	//		);
	//	}
	//}
}
