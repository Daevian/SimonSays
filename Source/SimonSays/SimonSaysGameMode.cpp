// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SimonSays.h"
#include "SimonSaysGameMode.h"
#include "SimonSaysCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Room.h"
#include "MainCharacter.h"

ASimonSaysGameMode::ASimonSaysGameMode()
{
    // set default pawn class to our character
    DefaultPawnClass = AMainCharacter::StaticClass();
}

void ASimonSaysGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> actorsFound;
    if (auto* world = GetWorld())
    {
        UGameplayStatics::GetAllActorsOfClass(world, ARoom::StaticClass(), actorsFound);
    }

    for (auto actor : actorsFound)
    {
        if (auto* room = Cast<ARoom>(actor))
        {
            m_rooms.AddUnique(room);
        }
    }

    if (auto* character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        if (m_rooms.Num() > 0)
        {
            character->SetCurrentRoom(m_rooms[0]);
        }
    }

    
}

void ASimonSaysGameMode::Tick(float time)
{
    
}
