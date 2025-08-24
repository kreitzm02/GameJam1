// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam1GameMode.h"
#include "GameJam1Character.h"
#include "UObject/ConstructorHelpers.h"

AGameJam1GameMode::AGameJam1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
