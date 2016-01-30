// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "Room.h"




ARoom::ARoom()
{
}

ARoom::~ARoom()
{
}

float ARoom::GetFloorZ() const
{
    float roomHeight = GetRenderComponent()->GetSprite()->GetSourceSize().Y;
    return GetActorLocation().Z - roomHeight * 0.5f + m_floorXOffset;
}
