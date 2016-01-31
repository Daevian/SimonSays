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
                const float c_wallXPos = 30.0f;
                FVector posInRoom = m_character->GetRelativePositionInRoom();
                if (!room->GetNeighbour(RoomNeighbour::Left))
                {
                    
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
