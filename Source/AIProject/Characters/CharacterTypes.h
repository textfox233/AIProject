#pragma once

//enum ECharacterState
//{
//	ECS_Dead
//};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Interrupted UMETA(DisplayName = "Interrupted"),
	EAS_Dead UMETA(DisplayName = "Dead")
};