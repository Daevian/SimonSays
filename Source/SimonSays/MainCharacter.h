// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

UCLASS()
class SIMONSAYS_API AMainCharacter : public APawn
{
    GENERATED_BODY()

private:
    UPROPERTY(Category = "Character", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPaperFlipbookComponent* m_sprite;


//////////////////////////////////////////////////////////////////////////

public:
    AMainCharacter();

    virtual void BeginPlay() override;
    
    virtual void Tick(float deltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;

    virtual void PostInitializeComponents() override;

    FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return m_sprite; }
    

private:
    static FName SpriteComponentName;
    
};
