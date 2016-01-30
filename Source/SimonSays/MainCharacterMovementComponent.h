// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "MainCharacterMovementComponent.generated.h"

enum class FacingDirection
{
    Left,
    Right,
};

UCLASS()
class SIMONSAYS_API UMainCharacterMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()
 
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (BlueprintProtected = "true"))
    float m_walkSpeed = 300.0f;



//////////////////////////////////////////////////////////////////////////
public:
    UMainCharacterMovementComponent();

    FacingDirection GetDirection() const { return m_direction; }
    
protected:

    virtual void InitializeComponent() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
    FacingDirection m_direction = FacingDirection::Right;
    
};
