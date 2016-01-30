// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include "Room.generated.h"

/**
 * 
 */
UCLASS()
class SIMONSAYS_API ARoom : public APaperSpriteActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (BlueprintProtected = "true"))
    float m_floorXOffset = 70.0f;

    
public:
    ARoom();
    ~ARoom();

    float GetFloorZ() const;
    
    
    
};
