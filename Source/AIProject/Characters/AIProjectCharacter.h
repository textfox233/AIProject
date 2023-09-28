#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "AIProjectCharacter.generated.h"


UCLASS(config=Game)
class AAIProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Health Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	/** Input Mapping Context */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction;

	EActionState ActionState = EActionState::EAS_Unoccupied;

public:
	AAIProjectCharacter();

	/** Melee Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UMeleeComponent* MeleeComponent;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for testing input */
	void TestSomething(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:

	// Setters/Getters
	UFUNCTION(BlueprintCallable, Category = CharacterStates)
	FORCEINLINE void SetActionState(EActionState NewState) { ActionState = NewState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

