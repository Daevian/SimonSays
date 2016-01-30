// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SimonSays.h"
#include "SimonSaysGameMode.h"
#include "SimonSaysCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Room.h"
#include "MainCharacter.h"
#include "RoomGrid.h"

ASimonSaysGameMode::ASimonSaysGameMode()
{
    DefaultPawnClass = AMainCharacter::StaticClass();
}

ASimonSaysGameMode::~ASimonSaysGameMode()
{
}

void ASimonSaysGameMode::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void ASimonSaysGameMode::BeginPlay()
{
    Super::BeginPlay();
    
}

void ASimonSaysGameMode::Tick(float time)
{
    
}


