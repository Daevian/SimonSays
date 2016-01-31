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
    bool m_hasDoorToTheLeft = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    bool m_hasDoorToTheRight = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    bool m_hasLadderUp = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    bool m_hasLadderDown = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_leftWallXPos = -700;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_rightWallXPos = 700;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_ladderPos = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_gridX = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Room", Meta = (BlueprintProtected = "true"))
    int32 m_gridY = 0;

    
//////////////////////////////////////////////////////////////////////////

public:

    static const int c_width = 2048;
    static const int c_height = 1024;

    ARoom();
    ~ARoom();

    float GetFloorZ() const;

    int32 GetGridX() const { return m_gridX; }
    int32 GetGridY() const { return m_gridY; }
    float IsStartingRoom() const { return m_isStartingRoom; }

    void SetNeighbour(ARoom* room, RoomNeighbour neighbour);
    ARoom* GetNeighbour(RoomNeighbour index);

    bool HasDoorToTheLeft() const   { return m_hasDoorToTheLeft; }
    bool HasDoorToTheRight() const  { return m_hasDoorToTheRight; }
    bool HasLadderUp() const        { return m_hasLadderUp; }
    bool HasLadderDown() const      { return m_hasLadderDown; }
    int32 GetLeftWallXPos() const   { return m_leftWallXPos;  }
    int32 GetRightWallXPos() const  { return m_rightWallXPos; }
    int32 GetLadderPos() const      { return m_ladderPos; }
    
private:
    std::array<ARoom*, static_cast<size_t>(RoomNeighbour::Count)> m_neighbours;
    
};
