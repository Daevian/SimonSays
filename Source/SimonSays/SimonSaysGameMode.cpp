// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SimonSays.h"
#include "SimonSaysGameMode.h"
#include "SimonSaysCharacter.h"

ASimonSaysGameMode::ASimonSaysGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = ASimonSaysCharacter::StaticClass();	
}
