// Fill out your copyright notice in the Description page of Project Settings.


#include "StringOutputter.h"

#include "RoomGenerator.h"


// Sets default values
AStringOutputter::AStringOutputter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStringOutputter::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawna la stanza centrale inoltre spawna con una probabilità del 50% le quattro stanze attorno ad essa
	PreconfigureFloor();


    // A partire da ognuna delle quattro stanze attorno a quella centrale sviluppa un percorso di stanze.
    // I percorsi saranno contigui, non si intersecano tra loro e le stanze non formano dei loops.
    int path = 1;

    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
        int roomX = ((floorSize - 1) / 2) - (int)round(sin(angle * PI));
        int roomY = ((floorSize - 1) / 2) + (int)round(cos(angle * PI));

        if (pathsMatrix[roomX][roomY] != -1) {
            roomsQueue.Emplace(TTuple<int, int>(roomX, roomY));
            indexOffset = -1;
            while (roomsQueue.Num() != 0) {
                BuildPath(get<0>(roomsQueue[roomsQueue.Num() - 1]), get<1>(roomsQueue[roomsQueue.Num() - 1]), path);
                roomsQueue.RemoveAt(roomsQueue.Num() - 1);
                indexOffset = -1;
            }
        }
        path++;
    }


    // Determina il numero e la posizione delle porte che saranno presenti in ogni stanza
    SetupDoors();


    // Spawna al centro di ogni stanza un attore "RoomGenerator" il cui scopo è quello di spawnare
    // gli elementi interni alla stanza (muri, porte, pavimento, nemici, etc)
    ARoomGenerator* spawnedRoom;
    FActorSpawnParameters spawnParams;
    int roomPosX, roomPosY;

    int offset = (floorSize - 1) / 2;
    for (int row = 0; row < floorSize; row++) {
        for (int col = 0; col < floorSize; col++) {
            if (pathsMatrix[row][col] >= 0) {
                roomPosX = (col - offset) * (floorWidth + 2 * wallWidth);
                roomPosY = (row - offset) * (floorWidth + 2 * wallWidth);
                spawnedRoom = GetWorld()->SpawnActor<ARoomGenerator>(FVector(roomPosX, roomPosY, 100), FRotator(0, 0, 0), spawnParams);
                spawnedRoom->GenerateFloor(valuesMatrix[row][col]);
            }
        }
    }
}

// Called every frame
void AStringOutputter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    
}

