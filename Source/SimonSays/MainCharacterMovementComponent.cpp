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

void UMainCharacterMovementComponent::RequestClimbUp()
{
    m_wantsToClimbUp = true;
}

void UMainCharacterMovementComponent::RequestClimbDown()
{
    m_wantsToClimbDown = true;
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

    if (m_character)
    {
        const FVector posInRoom = m_character->GetRelativePositionInRoom(); // pos should be center of room

        if (auto* room = m_character->GetCurrentRoom())
        {
            int ladderPosInRoom = room->GetLadderPos();
            bool isInsideLadderRegion = abs(posInRoom.X - ladderPosInRoom) <= (ARoom::c_ladderWidth * 0.5);
            
            // handle climbing up
            if (isInsideLadderRegion)
            {
                if (m_wantsToClimbUp)
                {
                    if (room->HasLadderUp())
                    {
                        if (auto* neighbour = room->GetNeighbour(RoomNeighbour::Up))
                        {
                            m_isMoving = false;
                            m_character->TeleportToRoom(neighbour);
                        }
                    }
                }

                // handle climbing down
                if (m_wantsToClimbDown)
                {
                    if (room->HasLadderDown())
                    {
                        if (auto* neighbour = room->GetNeighbour(RoomNeighbour::Down))
                        {
                            m_isMoving = false;
                            m_character->TeleportToRoom(neighbour);
                        }
                    }
                }
            }

            // handle left and right
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


                const float leftWallPos = room->GetLeftWallXPos();
                const float rightWallPos = room->GetRightWallXPos();

                FVector desiredPosInRoom = posInRoom - desiredMovementThisFrame;

                // no neighbour == wall
                // poor collision detection that won't work in low FPS (but hey, who gives a fucks?)
                // hint: not me
                if (!room->GetNeighbour(RoomNeighbour::Left) &&
                    desiredMovementThisFrame.X < 0 &&
                    desiredPosInRoom.X < leftWallPos)
                {
                    desiredMovementThisFrame.X = 0;
                    m_isMoving = false;

                }

                if (!room->GetNeighbour(RoomNeighbour::Right) &&
                    desiredMovementThisFrame.X > 0 &&
                    desiredPosInRoom.X > rightWallPos)
                {
                    desiredMovementThisFrame.X = 0;
                    m_isMoving = false;
                }

                FHitResult hit;
                MoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation().Quaternion(), true, &hit);
            }
            else
            {
                m_isMoving = false;
            }
        }
    }

    m_wantsToClimbUp = false;
    m_wantsToClimbDown = false;
}
