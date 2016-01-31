// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ActionActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMONSAYS_API UActionActorComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action", Meta = (BlueprintProtected = "true"))
		bool m_isCritical;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Action", Meta = (BlueprintProtected = "true"))
	class USoundBase* m_soundFx;

public:	
	// Sets default values for this component's properties
	UActionActorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void PlaySoundEffect();
	
};
