// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "RoomGrid.h"
#include "Room.h"
#include "MainCharacter.h"

const float ARoomGrid::c_roomSpriteDepth = -20.0f;

ARoomGrid::ARoomGrid() 
{
}

void ARoomGrid::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    m_roomGrid.SetNum(m_width);
    for (auto& column : m_roomGrid)
    {
        column.SetNumZeroed(m_height);
    }

    PopulateRoomGrid();
}

void ARoomGrid::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> actorsFound;
    if (auto* world = GetWorld())
    {
        UGameplayStatics::GetAllActorsOfClass(world, ARoom::StaticClass(), actorsFound);
    }

    if (auto* character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        for (auto* actor : actorsFound)
        {
            if (auto* room = Cast<ARoom>(actor))
            {
                if (room->IsStartingRoom())
                {
                    character->SetCurrentRoom(room);
                    break;
                }
            }
        }
    }
    
}

void ARoomGrid::Tick(float time)
{
    if (m_debugDrawBounds)
    {
        if (auto* world = GetWorld())
        {
            FVector topLeft = GetActorLocation();
            FVector topRight = topLeft + FVector(m_width * c_texWidth, 0.0f, 0.0f);
            FVector bottomLeft = topLeft + FVector(0.0f, m_height * c_texHeight, 0.0f);
            FVector bottomRight = topLeft + FVector(m_width * c_texWidth, m_height * c_texHeight, 0.0f);

            DrawDebugLine(world, topLeft,       topRight,       FColor(255, 0, 0), false, -1.0f, 0, 12.333f);
            DrawDebugLine(world, topRight,      bottomRight,    FColor(255, 0, 0), false, -1.0f, 0, 12.333f);
            DrawDebugLine(world, bottomRight,   bottomLeft,     FColor(255, 0, 0), false, -1.0f, 0, 12.333f);
            DrawDebugLine(world, bottomLeft,    topLeft,        FColor(255, 0, 0), false, -1.0f, 0, 12.333f);
        }
    }

}

void ARoomGrid::PopulateRoomGrid()
{
    if (auto* world = GetWorld())
    {
        for (TSubclassOf<ARoom> roomBP : m_roomBlueprints)
        {
            if (roomBP)
            {
                FActorSpawnParameters spawnParams;
                spawnParams.Owner = this;
                spawnParams.Instigator = this->Instigator;

                const FVector location(0.0f);
                const FRotator rotation(0);
                ARoom* room = world->SpawnActor<ARoom>(roomBP, location, rotation, spawnParams);
                
                bool roomPlacedInGrid = SetRoom(room, room->GetGridX(), room->GetGridY());
                if (!roomPlacedInGrid)
                {
                    world->RemoveActor(room, false);
                    room = nullptr;
                }
            }
        }

        SetNeighbours();
        PositionRooms();
    }
}

bool ARoomGrid::SetRoom(ARoom* room, int gridX, int gridY)
{
    if (gridX >= m_width ||
        gridY >= m_height)
    {
        return false;
    }

    auto*& slot = m_roomGrid[gridX][gridY];
    if (slot)
    {
        return false;
    }

    slot = room;
    return true;
}

void ARoomGrid::SetNeighbours()
{
    for (int gridX = 0; gridX > m_width; gridX++)

    {
        for (int gridY = 0; gridY > m_height; gridY++)
        {
            if (auto* room = m_roomGrid[gridX][gridY])
            {
                if (gridX - 1 >= 0)
                {
                    room->SetNeighbour(m_roomGrid[gridX - 1][gridY], RoomNeighbour::Left);
                }

                if (gridX + 1 < m_width)
                {
                    room->SetNeighbour(m_roomGrid[gridX + 1][gridY], RoomNeighbour::Right);
                }

                if (gridY - 1 > 0)
                {
                    room->SetNeighbour(m_roomGrid[gridX][gridY - 1], RoomNeighbour::Down);
                }

                if (gridY + 1 <= m_height)
                {
                    room->SetNeighbour(m_roomGrid[gridX][gridY + 1], RoomNeighbour::Up);
                }
            }
        }
    }


    // check if door are adjoint
}

void ARoomGrid::PositionRooms()
{
    int posX = 0;

    for (int gridX = 0; gridX < m_width; gridX++)
    {
        int posY = 0;
        for (int gridY = 0; gridY < m_height; gridY++)
        {
            if (auto* room = m_roomGrid[gridX][gridY])
            {
                FVector position((float)posX, (float)posY, c_roomSpriteDepth);
                room->SetActorLocation(position);
            }

            posY += c_texHeight;
        }

        posX += c_texWidth;
    }

    // shift position of rooms to fit
}
