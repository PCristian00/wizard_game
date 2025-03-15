// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "RoomGenerator.h"
#include "CentralRoomGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavMesh/RecastNavMesh.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    /*
    FActorSpawnParameters spawnInfo;
    if (GetWorld() != nullptr) {
        AActor* naviMesh = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(GetActorLocation(), GetActorRotation(), spawnInfo);

    }
    */
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
    Super::BeginPlay();


    // Spawna la stanza centrale inoltre spawna con una probabilità del 50% le quattro stanze attorno ad essa
    PreconfigureLevel();


    // A partire da ognuna delle quattro stanze attorno a quella centrale sviluppa un percorso di stanze.
    // I percorsi saranno contigui, non si intersecano tra loro e le stanze non formano dei loops.
    int path = 1;

    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
        int roomX = ((levelSize - 1) / 2) - (int)round(sin(angle * PI));
        int roomY = ((levelSize - 1) / 2) + (int)round(cos(angle * PI));

        if (roomsPathsMatrix[roomX][roomY] != -1) {
            roomsQueue.Emplace(TTuple<int, int>(roomX, roomY));
            indexOffset = -1;
            roomsInCurrentPath = 0;
            while (roomsQueue.Num() != 0) {
                BuildRoomsPath(get<0>(roomsQueue[roomsQueue.Num() - 1]), get<1>(roomsQueue[roomsQueue.Num() - 1]), path);
                if (roomsInCurrentPath > roomsInLongestPath) {
                    roomsInLongestPath = roomsInCurrentPath;
                    portalRoomX = get<0>(roomsQueue[roomsQueue.Num() - 1]);
                    portalRoomY = get<1>(roomsQueue[roomsQueue.Num() - 1]);
                }
                roomsQueue.RemoveAt(roomsQueue.Num() - 1);
                indexOffset = -1;
                roomsInCurrentPath++;
            }
        }
        path++;
    }



    // Determina il numero e la posizione delle porte che saranno presenti in ogni stanza
    DetermineDoorsPosition();


    // Spawna al centro di ogni stanza un attore "RoomGenerator" il cui scopo è quello di spawnare
    // gli elementi interni alla stanza (muri, porte, pavimento, nemici, etc)
    // Inoltre stabilisce quale stanza conterrà il portale per passare al livello successivo
    ARoomGenerator* spawnedRoom;
    ACentralRoomGenerator* spawnedCentralRoom;
    FActorSpawnParameters spawnParams;
    int roomPosX, roomPosY, roomLength, offset;

    roomLength = floorTilesWidth * roomsLengthInTiles;
    offset = (levelSize - 1) / 2;

    for (int row = 0; row < levelSize; row++) {
        for (int col = 0; col < levelSize; col++) {
            if (roomsPathsMatrix[row][col] >= 0) {
                roomPosX = (col - offset) * (roomLength + wallsWidth);
                roomPosY = (row - offset) * (roomLength + wallsWidth);
                if (GetWorld() != nullptr) {
                    if (row == offset && col == offset) {
                        spawnedCentralRoom = GetWorld()->SpawnActor<ACentralRoomGenerator>(FVector(roomPosX, roomPosY, 0), FRotator(0, 0, 0), spawnParams);
                        spawnedCentralRoom->GenerateFloor(doorsPositionMatrix[row][col]);
                    }
                    else {
                        spawnedRoom = GetWorld()->SpawnActor<ARoomGenerator>(FVector(roomPosX, roomPosY, 0), FRotator(0, 0, 0), spawnParams);
                        if (portalRoomX == row && portalRoomY == col) {
                            spawnedRoom->roomContainsPortal = true;
                        }
                        spawnedRoom->GenerateFloor(doorsPositionMatrix[row][col]);
                    }
                }
            }
        }
    }

    // Dopo aver spawnato le stanze distruggi le porte nella stanza centrale (che non conterrà nemici)
    AActor* centralRoomGenerator = UGameplayStatics::GetActorOfClass(GetWorld(), ACentralRoomGenerator::StaticClass());

    Cast<ACentralRoomGenerator>(centralRoomGenerator)->SetupDoors();
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

