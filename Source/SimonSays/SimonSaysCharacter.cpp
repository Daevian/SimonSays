// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SimonSays.h"
#include "SimonSaysCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "MainCharacterMovementComponent.h"



DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);
//////////////////////////////////////////////////////////////////////////
// ASimonSaysCharacter

ASimonSaysCharacter::ASimonSaysCharacter(const FObjectInitializer& ObjectInitializer)
    //: Super(ObjectInitializer.SetDefaultSubobjectClass<UMainCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Setup the assets
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
        FConstructorStatics()
            : RunningAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/RunningAnimation.RunningAnimation"))
            , IdleAnimationAsset(TEXT("/Game/2dSideScroller/Sprites/IdleAnimation.IdleAnimation"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;

    RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
    IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
    GetSprite()->SetFlipbook(IdleAnimation);

    // Use only Yaw from the controller and ignore the rest of the rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // Set the size of our collision capsule.
    GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
    GetCapsuleComponent()->SetCapsuleRadius(40.0f);

    // Create a camera boom attached to the root (capsule)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->AttachTo(RootComponent);
    CameraBoom->TargetArmLength = 500.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
    CameraBoom->bAbsoluteRotation = true;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
    

    // Create an orthographic camera (no perspective) and attach it to the boom
    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 2048.0f;
    SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

    // Prevent all automatic rotation behavior on the camera, character, and camera component
    CameraBoom->bAbsoluteRotation = true;
    SideViewCameraComponent->bUsePawnControlRotation = false;
    SideViewCameraComponent->bAutoActivate = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    // Configure character movement
    GetCharacterMovement()->GravityScale = 0.0f;
    GetCharacterMovement()->AirControl = 0.0f;
    GetCharacterMovement()->JumpZVelocity = 0.0f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxFlySpeed = 600.0f;

    // Lock character motion onto the XZ plane, so the character can't move in or out of the screen
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

    // Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
    // Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
    // behavior on the edge of a ledge versus inclines by setting this to true or false
    GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

// 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
// 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
// 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
// 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
// 	TextComponent->AttachTo(RootComponent);

    // Enable replication on the Sprite component so animations show up when networked
    GetSprite()->SetIsReplicated(true);
    bReplicates = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASimonSaysCharacter::UpdateAnimation()
{
    const FVector PlayerVelocity = GetVelocity();
    const float PlayerSpeed = PlayerVelocity.Size();

    // Are we moving or standing still?
    UPaperFlipbook* DesiredAnimation = (PlayerSpeed > 0.0f) ? RunningAnimation : IdleAnimation;
    if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
    {
        GetSprite()->SetFlipbook(DesiredAnimation);
    }
}

void ASimonSaysCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    UpdateCharacter();
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASimonSaysCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    // Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
    //InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    //InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    InputComponent->BindAxis("MoveRight", this, &ASimonSaysCharacter::MoveRight);

    //InputComponent->BindTouch(IE_Pressed, this, &ASimonSaysCharacter::TouchStarted);
    //InputComponent->BindTouch(IE_Released, this, &ASimonSaysCharacter::TouchStopped);
}

void ASimonSaysCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

//void ASimonSaysCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	// jump on any touch
//	Jump();
//}
//
//void ASimonSaysCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	StopJumping();
//}

void ASimonSaysCharacter::UpdateCharacter()
{
    UpdateAnimation();
    
    const FVector playerVelocity = GetVelocity();	
    float travelDirection = playerVelocity.X;
    
    if (Controller != nullptr)
    {
        if (travelDirection < 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
        }
        else if (travelDirection > 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        }
    }
}

void ASimonSaysCharacter::SetCurrentRoom(ARoom* room)
{
    m_currentRoom = room;
}

