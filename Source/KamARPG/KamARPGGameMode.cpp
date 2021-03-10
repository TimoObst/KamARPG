// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "KamARPGGameMode.h"
#include "KamARPGPlayerController.h"
#include "KamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKamARPGGameMode::AKamARPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AKamARPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/KamARPG/Core/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}