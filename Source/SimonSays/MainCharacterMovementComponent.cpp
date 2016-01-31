// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "MainCharacterMovementComponent.h"
#include "MainCharacter.h"
#include "Room.h"



UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
}

void UMainCharacterMovementComponent::SetCharacter(AMainCharacter* character)
{
    m_character = character;
}

void UMainCharacterMovementComponent::InitializeComponent()
{
    Super::InitializeComponent();
}

void UMainCharacterMovementComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction)
{
    UPawnMovementComponent::TickComponent(deltaTime, tickType, thisTickFunction);

    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(deltaTime))
    {
        return;
    }

    FVector desiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * deltaTime * m_walkSpeed;
    if (!desiredMovementThisFrame.IsNearlyZero())
    {
        m_isMoving = true;
        if (desiredMovementThisFrame.X < 0.0f)
        {
            m_direction = FacingDirection::Left;
        }
        else if (desiredMovementThisFrame.X > 0.0f)
        {
            m_direction = FacingDirection::Right;
        }

        if (m_character)
        {
            if (auto* room = m_character->GetCurrentRoom())
            {
                
                const float halfRoomWidthWithWall = ARoom::c_width * 0.5f - ARoom::c_wallXPos;

                FVector posInRoom = m_character->GetRelativePositionInRoom(); // pos should be center of room
                FVector desiredPosInRoom = posInRoom - desiredMovementThisFrame;

                // no neighbour == wall
                // poor collision detection that won't work in low FPS (but hey, who gives a fucks?)
                // hint: not me
                if (!room->GetNeighbour(RoomNeighbour::Left) &&
                    desiredMovementThisFrame.X < 0 &&
                    desiredPosInRoom.X < -halfRoomWidthWithWall)
                {
                    desiredMovementThisFrame.X = 0;// (-halfRoomWidthWithWall) - desiredPosInRoom.X;
                    m_isMoving = false;

                }

                if (!room->GetNeighbour(RoomNeighbour::Right) &&
                    desiredMovementThisFrame.X > 0 &&
                    desiredPosInRoom.X > halfRoomWidthWithWall)
                {
                    desiredMovementThisFrame.X = 0;// halfRoomWidthWithWall - desiredPosInRoom.X;
                    m_isMoving = false;
                }
            }            

        }

        FHitResult hit;
        MoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation().Quaternion(), true, &hit);
    }
    else
    {
        m_isMoving = false;
    }
}
