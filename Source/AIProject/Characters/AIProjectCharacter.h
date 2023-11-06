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
	class UInputAction* TestAction1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TestAction3;

	EActionState ActionState = EActionState::EAS_Unoccupied;

	/** Debug booleans */
	//UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Debug, meta = (AllowPrivateAccess = "true"))
	//bool bDrawDebug = false;
	//UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = Debug, meta = (AllowPrivateAccess = "true"))
	//bool bDebugLog = false;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Debug|Character", meta = (AllowPrivateAccess = "true"))
	bool bDebugMsg = false;

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
	void TestFunction1(const FInputActionValue& Value);
	void TestFunction2(const FInputActionValue& Value);
	void TestFunction3(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// Debug messages
	virtual void Tick(float DeltaSeconds) override;

public:

	// Setters/Getters
	UFUNCTION(BlueprintCallable, Category = CharacterStates)
	//FORCEINLINE void SetActionState(EActionState NewState) { ActionState = NewState; }
	void SetActionState(EActionState NewState = EActionState::EAS_Unoccupied);
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	
	void CheckActionState();
};

