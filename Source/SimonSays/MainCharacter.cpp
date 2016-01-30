// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "MainCharacter.h"
#include "PaperFlipbookComponent.h"
#include "MainCharacterMovementComponent.h"

FName AMainCharacter::c_spriteComponentName(TEXT("Sprite0"));
FName AMainCharacter::c_movementComponentName(TEXT("MainMoveComp"));

// Sets default values
AMainCharacter::AMainCharacter(const class FObjectInitializer& objectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

    m_movementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>(c_movementComponentName);
    m_movementComponent->UpdatedComponent = this->RootComponent;

    m_sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(c_spriteComponentName);
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

    AutoPossessPlayer = EAutoReceiveInput::Player0;
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

    InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

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

UPawnMovementComponent* AMainCharacter::GetMovementComponent() const
{
    return m_movementComponent;
}

void AMainCharacter::MoveRight(float axisValue)
{
    if (m_movementComponent && (m_movementComponent->UpdatedComponent == RootComponent))
    {
        const FVector right(1.0f, 0.0f, 0.0f);
        m_movementComponent->AddInputVector(right * axisValue);
    }
}
