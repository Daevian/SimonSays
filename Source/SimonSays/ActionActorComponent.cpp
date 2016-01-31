// Fill out your copyright notice in the Description page of Project Settings.

#include "SimonSays.h"
#include "ActionActorComponent.h"


// Sets default values for this component's properties
UActionActorComponent::UActionActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UActionActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UActionActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UActionActorComponent::PlaySoundEffect()
{
	UGameplayStatics::PlaySound2D(this, m_soundFx);
}

