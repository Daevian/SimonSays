// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "Room.h"
//#include "PaperSprite.h"




ARoom::ARoom()
{
    m_neighbours = { nullptr, nullptr, nullptr, nullptr };
}

ARoom::~ARoom()
{
}

float ARoom::GetFloorZ() const
{
    //if (UPaperSpriteComponent* renderComp = GetRenderComponent())
    //
    //   if (UPaperSprite* sprite = renderComp->GetSprite())
    //   {
    //       float roomHeight = sprite->GetSourceSize().Y;
    //       return GetActorLocation().Z - roomHeight * 0.5f + m_floorXOffset;            
    //   }
    //
    //return 0.0f;

    // hack - damn UPaperSprite says it doesn't have GetSourceSize() WHICH IT DEFINITELY HAS!
    // assume the room is 2048x1024

    float roomHeight = 1024.0f;
    return GetActorLocation().Z - roomHeight * 0.5f + m_floorXOffset;            



    return 0.0f;
}

void ARoom::SetNeighbour(ARoom* room, RoomNeighbour neighbour)
{
    m_neighbours[static_cast<size_t>(neighbour)] = room;
}
