// Copyright Epic Games, Inc. All Rights Reserved.

#include "VisualStudioTestGameMode.h"
#include "VisualStudioTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVisualStudioTestGameMode::AVisualStudioTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
