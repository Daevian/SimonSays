// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "MainCharacterMovementComponent.h"



UMainCharacterMovementComponent::UMainCharacterMovementComponent()
{
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

    FVector desiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * deltaTime * 150.0f;
    if (!desiredMovementThisFrame.IsNearlyZero())
    {
        FHitResult hit;
        SafeMoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, hit);

        if (desiredMovementThisFrame.X < 0.0f)
        {
            m_direction = FacingDirection::Left;
        }
        else if (desiredMovementThisFrame.X > 0.0f)
        {
            m_direction = FacingDirection::Right;
        }
    }
}
