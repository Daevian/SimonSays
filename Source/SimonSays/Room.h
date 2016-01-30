// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSpriteActor.h"
#include <array>
#include "Room.generated.h"

enum class RoomNeighbour
{
    Left,
    Right,
    Up,
    Down,
    Count,
};

UCLASS()
class SIMONSAYS_API ARoom : public APaperSpriteActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (BlueprintProtected = "true"))
    float m_floorXOffset = 70.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    bool m_isStartingRoom = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_gridX = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_gridY = 0;

    
//////////////////////////////////////////////////////////////////////////

public:
    ARoom();
    ~ARoom();

    float GetFloorZ() const;

    int32 GetGridX() const { return m_gridX; }
    int32 GetGridY() const { return m_gridY; }
    float IsStartingRoom() const { return m_isStartingRoom; }

    void SetNeighbour(ARoom* room, RoomNeighbour neighbour);
    
private:
    std::array<ARoom*, static_cast<size_t>(RoomNeighbour::Count)> m_neighbours;
    
};
