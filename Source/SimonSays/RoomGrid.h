// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MainCharacter.h"
#include "RoomGrid.generated.h"

class ARoom;

UCLASS()
class SIMONSAYS_API ARoomGrid : public AActor
{
    GENERATED_BODY()
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomGrid")
    TArray<TSubclassOf<ARoom>> m_roomBlueprints;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoomGrid")
    int32 m_width = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RoomGrid")
    int32 m_height = 0;

//////////////////////////////////////////////////////////////////////////


public:	
    ARoomGrid();
    virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
    virtual void Tick(float time) override;

    void PopulateRoomGrid();
    bool SetRoom(ARoom* room, int gridX, int gridY);
    void SetNeighbours();
    void PositionRooms();

	void UpdateCurrentRoom();

private:
    // hack - assume all rooms are 2048x1024, and that all rooms are in depth -20;
    static const int c_texWidth = 2048;
    static const int c_texHeight = 1024;
    static const float c_roomSpriteDepth;;

    TArray<TArray<ARoom*>> m_roomGrid;
	AMainCharacter* m_mainCharacter;
    
};
