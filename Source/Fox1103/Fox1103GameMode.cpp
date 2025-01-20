// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fox1103GameMode.h"
#include "Fox1103Character.h"
#include "UObject/ConstructorHelpers.h"

AFox1103GameMode::AFox1103GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
