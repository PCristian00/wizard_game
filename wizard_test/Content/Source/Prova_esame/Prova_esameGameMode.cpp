// Copyright Epic Games, Inc. All Rights Reserved.

#include "Prova_esameGameMode.h"
#include "Prova_esameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProva_esameGameMode::AProva_esameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
