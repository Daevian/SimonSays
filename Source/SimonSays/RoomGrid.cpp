// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "RoomGrid.h"
#include "Room.h"
#include "MainCharacter.h"

const float ARoomGrid::c_roomSpriteDepth = -20.0f;

ARoomGrid::ARoomGrid() 
{
    PrimaryActorTick.bCanEverTick = true;
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
        m_mainCharacter = character;
        for (auto* actor : actorsFound)
        {
            if (auto* room = Cast<ARoom>(actor))
            {
                if (room->IsStartingRoom())
                {
                    m_mainCharacter->SetCurrentRoom(room);

                    // teleport to center of starting room
                    auto characterPos = m_mainCharacter->GetActorLocation();
                    auto roomPos = room->GetActorLocation();

                    characterPos.X = roomPos.X;
                    characterPos.Z = roomPos.Z;

                    m_mainCharacter->SetActorLocation(characterPos);
                    break;
                }
            }
        }
    }
    
}

void ARoomGrid::Tick(float time)
{
    /*if (m_debugDrawBounds)
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
    }*/

    UpdateCurrentRoom();
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

                const FVector location = this->GetActorLocation();
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
    for (int gridX = 0; gridX < m_width; gridX++)

    {
        for (int gridY = 0; gridY < m_height; gridY++)
        {
            if (auto* room = m_roomGrid[gridX][gridY])
            {
                // left
                if (gridX - 1 >= 0 &&
                    room->HasDoorToTheLeft())
                {
                    room->SetNeighbour(m_roomGrid[gridX - 1][gridY], RoomNeighbour::Left);
                }

                // right
                if (gridX + 1 < m_width &&
                    room->HasDoorToTheRight())
                {
                    room->SetNeighbour(m_roomGrid[gridX + 1][gridY], RoomNeighbour::Right);
                }

                // down
                if (gridY - 1 > 0 &&
                    room->HasLadderDown())
                {
                    room->SetNeighbour(m_roomGrid[gridX][gridY - 1], RoomNeighbour::Down);
                }

                // up
                if (gridY + 1 < m_height &&
                    room->HasLadderUp())
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
    const FVector location = this->GetActorLocation();
    float posX = location.X;

    for (int gridX = 0; gridX < m_width; gridX++)
    {
        float posZ = location.Z;
        for (int gridY = 0; gridY < m_height; gridY++)
        {
            if (auto* room = m_roomGrid[gridX][gridY])
            {
                FVector position(posX, c_roomSpriteDepth, posZ);
                room->SetActorLocation(position);
            }

            posZ -= c_texHeight;
        }

        posX += c_texWidth;
    }

    // shift position of rooms to fit
}

void ARoomGrid::UpdateCurrentRoom()
{
    ARoom* currentRoom = m_mainCharacter->GetCurrentRoom();
    for (int i = 0; i < 4; i++)
    {
        ARoom* neighbour = currentRoom->GetNeighbour(static_cast<RoomNeighbour>(i));
        if (neighbour)
        {
            //Boundaries check
            FVector characterPos = m_mainCharacter->GetActorLocation();
            FVector roomPos = neighbour->GetActorLocation();
            if (characterPos.X >= roomPos.X - c_texWidth/2
                && characterPos.X <= roomPos.X + c_texWidth/2
                && characterPos.Z <= roomPos.Z + c_texHeight/2
                && characterPos.Z >= roomPos.Z - c_texHeight/2)
            {
                currentRoom = neighbour;
                m_mainCharacter->SetCurrentRoom(currentRoom);
                return;
            }
        }
    }
}
