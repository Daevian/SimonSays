// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "MainCharacterMovementComponent.generated.h"

class AMainCharacter;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (BlueprintProtected = "true"))
    bool m_isMoving = false;



//////////////////////////////////////////////////////////////////////////
public:
    UMainCharacterMovementComponent();

    FacingDirection GetDirection() const { return m_direction; }

    void SetCharacter(AMainCharacter* character);
    
protected:

    virtual void InitializeComponent() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
    FacingDirection m_direction = FacingDirection::Right;

    AMainCharacter* m_character = nullptr;
    
};
