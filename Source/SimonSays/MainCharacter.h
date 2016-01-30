// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

class UMainCharacterMovementComponent;

UCLASS()
class SIMONSAYS_API AMainCharacter : public APawn
{
    GENERATED_BODY()

private:
    UPROPERTY(Category = "Character", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPaperFlipbookComponent* m_sprite = nullptr;

    UPROPERTY(Category = "Character", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    UMainCharacterMovementComponent* m_movementComponent = nullptr;


//////////////////////////////////////////////////////////////////////////

public:
    AMainCharacter(const class FObjectInitializer& objectInitializer);

    virtual void BeginPlay() override;
    
    virtual void Tick(float deltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;

    virtual void PostInitializeComponents() override;

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return m_sprite; }


private:
    void MoveRight(float axisValue);

    static FName c_spriteComponentName;
    static FName c_movementComponentName;
    
    
    
};
