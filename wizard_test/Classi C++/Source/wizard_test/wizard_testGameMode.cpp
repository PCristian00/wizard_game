// Copyright Epic Games, Inc. All Rights Reserved.

#include "wizard_testGameMode.h"
#include "wizard_testCharacter.h"
#include "UObject/ConstructorHelpers.h"

Awizard_testGameMode::Awizard_testGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
