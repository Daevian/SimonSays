// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "MainCharacter.h"
#include "PaperFlipbookComponent.h"

FName AMainCharacter::SpriteComponentName(TEXT("Sprite0"));

// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    m_sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AMainCharacter::SpriteComponentName);
    if (m_sprite)
    {
        m_sprite->AlwaysLoadOnClient = true;
        m_sprite->AlwaysLoadOnServer = true;
        m_sprite->bOwnerNoSee = false;
        m_sprite->bAffectDynamicIndirectLighting = true;
        m_sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        m_sprite->AttachParent = this->RootComponent;
        static FName CollisionProfileName(TEXT("CharacterMesh"));
        m_sprite->SetCollisionProfileName(CollisionProfileName);
        m_sprite->bGenerateOverlapEvents = false;
    }
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
	Super::SetupPlayerInputComponent(inputComponent);

}

void AMainCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!IsPendingKill())
    {
        if (m_sprite)
        {
            // force animation tick after movement component updates
            if (m_sprite->PrimaryComponentTick.bCanEverTick && GetMovementComponent())
            {
                m_sprite->PrimaryComponentTick.AddPrerequisite(GetMovementComponent(), GetMovementComponent()->PrimaryComponentTick);
            }
        }
    }
}

