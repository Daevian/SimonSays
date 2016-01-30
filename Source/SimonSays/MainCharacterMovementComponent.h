// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "MainCharacterMovementComponent.generated.h"

UCLASS()
class SIMONSAYS_API UMainCharacterMovementComponent : public UFloatingPawnMovement
{
    GENERATED_BODY()
    
    
protected:

    virtual void InitializeComponent() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    
};
