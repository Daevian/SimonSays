// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
#include "SimonSaysCharacter.generated.h"

// This class is the default character for SimonSays, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

class UTextRenderComponent;
class ARoom;

UCLASS(config=Game)
class ASimonSaysCharacter : public APaperCharacter
{
    GENERATED_BODY()

protected:
    // The animation to play while running around
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* RunningAnimation;

    // The animation to play while idle (standing still)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* IdleAnimation;

private:
    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
    class UCameraComponent* SideViewCameraComponent;

    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;






///////////////////////////////////////////////////////////////////

public:
    ASimonSaysCharacter(const FObjectInitializer& ObjectInitializer);

    /** Returns SideViewCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    void SetCurrentRoom(ARoom* room);

protected:

    void UpdateAnimation();

    void MoveRight(float Value);

    void UpdateCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
    UTextRenderComponent* TextComponent;
    virtual void Tick(float DeltaSeconds) override;

    ARoom* m_currentRoom = nullptr;

};
