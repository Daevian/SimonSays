// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "MainCharacter.h"
#include "PaperFlipbookComponent.h"
#include "MainCharacterMovementComponent.h"
#include "Room.h"

FName AMainCharacter::c_spriteComponentName(TEXT("Sprite0"));
FName AMainCharacter::c_movementComponentName(TEXT("MainMoveComp"));

// Sets default values
AMainCharacter::AMainCharacter(const class FObjectInitializer& objectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

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

    m_movementComponent = CreateDefaultSubobject<UMainCharacterMovementComponent>(c_movementComponentName);
    m_movementComponent->UpdatedComponent = this->RootComponent;
    m_movementComponent->SetCharacter(this);

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
	InputComponent->BindAction("PerformAction", IE_Released, this, &AMainCharacter::DoAction);
    
}

// Called every frame
void AMainCharacter::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    UpdateCharacter();

}

void AMainCharacter::UpdateCharacter()
{
    SetCurrentRoom(m_currentRoom);
    UpdateDirection();
}

void AMainCharacter::UpdateDirection()
{
    if (m_movementComponent)
    {
        auto direction = m_movementComponent->GetDirection();

        if (this->Controller != nullptr)
        {
            switch (direction)
            {
                case FacingDirection::Left:
                {
                    this->Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
                } break;

                case FacingDirection::Right:
                {
                    this->Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
                } break;

                default:
                    break;
            }
        }
    }
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* inputComponent)
{
    Super::SetupPlayerInputComponent(inputComponent);

    InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
    InputComponent->BindAction("ClimbUp", IE_Pressed, this, &AMainCharacter::ClimbUp);
    InputComponent->BindAction("ClimbDown", IE_Pressed, this, &AMainCharacter::ClimbDown);

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

void AMainCharacter::SetCurrentRoom(ARoom* room)
{
    m_currentRoom = room;
    if (m_currentRoom)
    {
        float floorZ = m_currentRoom->GetFloorZ();
        FVector location = GetActorLocation();
        location.Z = floorZ;
//#if WITH_EDITORONLY_DATA
//        location += this->PivotOffset;
//#else
//        location += FVector(0.0f, 0.0f, 100.0); // hack - hardcoded pivot offset
//#endif
        SetActorLocation(location);
    }
}

UPawnMovementComponent* AMainCharacter::GetMovementComponent() const
{
    return m_movementComponent;
}

FVector AMainCharacter::GetRelativePositionInRoom() const
{
    if (auto* room = GetCurrentRoom())
    {
        FVector characterPos = GetActorLocation();
        FVector roomPos = room->GetActorLocation();
        return characterPos - roomPos;
    }

    return FVector(0.0f);
}

void AMainCharacter::TeleportToRoom(ARoom* room)
{
    if (room)
    {
        SetCurrentRoom(room);
    }
}

void AMainCharacter::MoveRight(float axisValue)
{
    if (m_movementComponent && (m_movementComponent->UpdatedComponent == RootComponent))
    {
        const FVector right(1.0f, 0.0f, 0.0f);
        m_movementComponent->AddInputVector(right * axisValue);
    }
}

void AMainCharacter::ClimbUp()
{
    if (m_movementComponent && (m_movementComponent->UpdatedComponent == RootComponent))
    {
        m_movementComponent->RequestClimbUp();
    }
}

void AMainCharacter::ClimbDown()
{
    if (m_movementComponent && (m_movementComponent->UpdatedComponent == RootComponent))
    {
        m_movementComponent->RequestClimbDown();
    }
}

void AMainCharacter::DoAction()
{
	if (nullptr != m_currentRoom)
	{
		m_currentRoom->DoAction();
	}
}
