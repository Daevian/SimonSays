// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

class UMainCharacterMovementComponent;
class ARoom;

UCLASS()
class SIMONSAYS_API AMainCharacter : public APawn
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Room")
    ARoom* GetCurrentRoom() const { return m_currentRoom; }

protected:
    // The animation to play while running around
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* m_runningAnimation = nullptr;

    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* m_idleAnimation = nullptr;

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

    void SetCurrentRoom(ARoom* room);

    virtual UPawnMovementComponent* GetMovementComponent() const override;

    FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return m_sprite; }

    FVector GetRelativePositionInRoom() const;

    void TeleportToRoom(ARoom* room);

private:
    void UpdateCharacter();
    void UpdateDirection();
    void MoveRight(float axisValue);
    void ClimbUp();
    void ClimbDown();

	void DoAction();

    static FName c_spriteComponentName;
    static FName c_movementComponentName;
    
    ARoom* m_currentRoom = nullptr;

    FString m_debugStr;
    
};
