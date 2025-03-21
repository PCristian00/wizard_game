// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "chrono"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DitchFloorTile.h"
#include "LavaFloorTile.h"
#include "FloorTileWithoutEffect.h"
#include "ClosedWall.h"
#include "OpenWall.h"
#include "Door.h"
#include "Portal.h"
#include "EnemySpawner.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerator.generated.h"

UCLASS()
class WIZARD_TEST_API ARoomGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static const int roomsLengthInTiles = 7; // lunghezza (espressa in tiles) del lato delle stanze
	static const int floorTilesWidth = 200; // lunghezza delle tiles quadrate componenti il pavimento delle stanze
	static const int floorTilesHeight = 100; // altezza delle tiles quadrate componenti il pavimento delle stanze
	static const int wallsWidth = 100; // spessore dei muri delle stanze
	static const int floorCentre = (roomsLengthInTiles - 1) / 2;
	int roomID, enemiesInTheRoom;
	int maxEnemiesInsideOneRoom = 10;
	int floorTilesTypes[roomsLengthInTiles][roomsLengthInTiles] = {};
	bool playerEnteredRoomAlready = false;
	bool roomContainsPortal = false;


	void GenerateFloor(int roomValue) {
		roomID = roomValue;
		int floorSubMatrix[floorCentre][floorCentre] = {};

		// Riempie con tutti -2 (tiles senza effetto) la matrice primaria e la sottomatrice
		for (int x = 0; x < roomsLengthInTiles; x++) {
			for (int y = 0; y < roomsLengthInTiles; y++) {
				floorTilesTypes[x][y] = -2;
			}
		}

		for (int x = 0; x < floorCentre; x++) {
			for (int y = 0; y < floorCentre; y++) {
				floorSubMatrix[x][y] = -2;
			}
		}

		// Crea il pattern nella sottomatrice in alto a destra
		switch (rand() % 3) {
		case 0:
			CreateFlowerPattern(floorSubMatrix);
			break;
		case 1:
			CreateCornersPattern(floorSubMatrix);
			break;
		case 2:
			CreateDiagonalPattern(floorSubMatrix);
			break;
		}

		// Ruota la sottomatrice in senso antiorario di 90°, 180° e 270° e copiala negli angoli della matrice principale
		FillMainMatrix(floorSubMatrix);

		// Crea dei percorsi (di tiles senza effetto) che congiungono le porte della stanza
		CreatePathsBetweenDoors(roomValue);

		// Spawna le mura attorno ad ogni stanza tenendo conto del numero e della posizione delle porte
		SpawnWalls(roomValue);

		// Spawna le porte nella stanza
		SpawnDoors(roomValue);

		// Spawna le tiles che compongono il pavimento (alcune tiles applicano un effetto al giocatore, altre no)
		SpawnFloorTiles();

		// Elimina i triggers spawnati davanti a delle mura senza porte
		DeleteUselessTriggers(roomValue);

		// Viene stabilito il numero di nemici da spawnare nella stanza
		srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		//enemiesInTheRoom = rand() % (maxEnemiesInsideOneRoom + 1);
		enemiesInTheRoom = 0;

		if (enemiesInTheRoom == 0 && roomContainsPortal) {
			//UE_LOG(LogTemp, Error, TEXT("The room containing the portal is called: %s"), *GetDebugName(this));
			FActorSpawnParameters spawnParams;
			FVector posOffset = FVector(0.0, 0.0, 95.0);
			GetWorld()->SpawnActor<APortal>(GetActorLocation() + posOffset, FRotator(0, 0, 0), spawnParams);
		}
	}


	// Ruota la sottomatrice in senso antiorario di 90°, 180° e 270° e copiala negli angoli della matrice principale
	void FillMainMatrix(int matrix[floorCentre][floorCentre]) {
		int offsetPosX[4] = { 0, 0, 1, 1 };
		int offsetPosY[4] = { 1, 0, 0, 1 };

		srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		for (int i = 0; i < 4; i++) {
			if (rand() % 2) {
				for (int x = 0; x < floorCentre; x++) {
					for (int y = 0; y < floorCentre; y++) {
						floorTilesTypes[x + offsetPosX[i] * (floorCentre + 1)][y + offsetPosY[i] * (floorCentre + 1)] = matrix[x][y];
					}
				}
			}
			RotateMatrix(matrix, 1);
		}
	}


	// Crea dei percorsi (di tiles senza effetto) che congiungono le porte della stanza
	void CreatePathsBetweenDoors(int roomValue) {
		int initialRoomValue = roomValue;

		if (roomValue % 2 == 1) {
			for (int tilePosY = floorCentre + 1; tilePosY < roomsLengthInTiles; tilePosY++) {
				floorTilesTypes[floorCentre][tilePosY] = -2;
			}
		}
		roomValue = roomValue / 2;

		if (roomValue % 2 == 1) {
			for (int tilePosX = 0; tilePosX < floorCentre; tilePosX++) {
				floorTilesTypes[tilePosX][floorCentre] = -2;
			}
		}
		roomValue = roomValue / 2;

		if (roomValue % 2 == 1) {
			for (int tilePosY = 0; tilePosY < floorCentre; tilePosY++) {
				floorTilesTypes[floorCentre][tilePosY] = -2;
			}
		}
		roomValue = roomValue / 2;

		if (roomValue == 1) {
			for (int tilePosX = floorCentre + 1; tilePosX < roomsLengthInTiles; tilePosX++) {
				floorTilesTypes[tilePosX][floorCentre] = -2;
			}
		}
	}


	// Spawna le mura attorno ad ogni stanza tenendo conto del numero e della posizione delle porte
	void SpawnWalls(int roomValue) {
		FActorSpawnParameters spawnParams;
		FVector spawnPos = GetActorLocation();
		int wallRotation = 0;
		double xPosOffset = (roomsLengthInTiles * floorTilesWidth) / 2 + wallsWidth / 2;
		int directionX, directionY;

		for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
			directionX = round(cos(angle * PI));
			directionY = -round(sin(angle * PI));
			if (roomValue % 2 == 1) {
				GetWorld()->SpawnActor<AOpenWall>(spawnPos + xPosOffset * FVector(directionX, directionY, 0), FRotator(0, wallRotation, 0), spawnParams);
			}
			else {
				GetWorld()->SpawnActor<AClosedWall>(spawnPos + xPosOffset * FVector(directionX, directionY, 0), FRotator(0, wallRotation, 0), spawnParams);
			}
			roomValue = roomValue / 2;
			wallRotation = wallRotation - 90;
		}
	}


	// Spawna le porte nella stanza
	void SpawnDoors(int roomValue) {
		FActorSpawnParameters spawnParams;
		FVector spawnPos = GetActorLocation() + FVector(0, 0, 100);
		int doorRotation = 0;
		double xPosOffset = (roomsLengthInTiles * floorTilesWidth) / 2 + wallsWidth / 2;

		for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
			if (roomValue % 2 == 1) {
				GetWorld()->SpawnActor<ADoor>(spawnPos + xPosOffset * FVector(round(cos(angle * PI)), -round(sin(angle * PI)), 0), FRotator(0, doorRotation, 0), spawnParams);
			}
			roomValue = roomValue / 2;
			doorRotation = doorRotation - 90;
		}
	}


	// Distrugge le porte nella stanza
	void DeleteDoors() {
		TArray<AActor*> doors;
		FVector doorPos;
		double maxDistance = roomsLengthInTiles * floorTilesWidth / 2 + wallsWidth;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), doors);

		for (AActor* door : doors) {
			doorPos = door->GetActorLocation();

			if (FVector::DistXY(doorPos, GetActorLocation()) <= maxDistance) {
				door->Destroy();
			}
		}
	}


	// Spawna le tiles che compongono il pavimento (alcune tiles applicano un effetto al giocatore, altre no)
	void SpawnFloorTiles() {
		FActorSpawnParameters spawnParams;
		FVector spawnPos = GetActorLocation();
		int tilesOffset = (roomsLengthInTiles - 1) / 2;
		int tilesPosX, tilesPosY;
		int tileEffectType = rand() % 2;

		for (int row = 0; row < roomsLengthInTiles; row++) {
			for (int col = 0; col < roomsLengthInTiles; col++) {
				tilesPosX = (col - tilesOffset) * floorTilesWidth;
				tilesPosY = (row - tilesOffset) * floorTilesWidth;

				if (floorTilesTypes[row][col] == -1) {
					switch (tileEffectType) {
					case 0:
						GetWorld()->SpawnActor<ALavaFloorTile>(spawnPos + FVector(tilesPosX, tilesPosY, -20), FRotator(0, 0, 0), spawnParams);
						break;
					case 1:
						GetWorld()->SpawnActor<ADitchFloorTile>(spawnPos + FVector(tilesPosX, tilesPosY, -600), FRotator(0, 0, 0), spawnParams);
						break;
					}
				}
				else {
					GetWorld()->SpawnActor<AFloorTileWithoutEffect>(spawnPos + FVector(tilesPosX, tilesPosY, 0), FRotator(0, 0, 0), spawnParams);
				}
			}
		}
	}


	// Elimina i triggers spawnati davanti a delle mura senza porte
	void DeleteUselessTriggers(int roomValue) {
		for (int i = 0; i < 4; i++) {
			if (roomValue % 2 != 1) {
				triggers[i]->DestroyComponent();
			}
			roomValue = roomValue / 2;
		}
	}



	/* Esempio del "flower pattern" su una matrice 9 x 9
	*	0 -> tiles senza effetto     2 -> tiles con bonus/malus
	*
	*	2 0 0 0 0 2 2 2 2
	*	2 2 0 0 0 2 2 2 0				Sottomatrice 4 x 4
	*	2 2 2 0 0 2 2 0 0					2 2 2 2
	*	2 2 2 2 0 2 0 0 0					2 2 2 0
	*	0 0 0 0 0 0 0 0 0					2 2 0 0
	*	0 0 0 2 0 2 2 2 2					2 0 0 0
	*	0 0 2 2 0 0 2 2 2
	*	0 2 2 2 0 0 0 2 2
	*	2 2 2 2 0 0 0 0 2
	*/
	void CreateFlowerPattern(int matrix[floorCentre][floorCentre]) {
		for (int x = 0; x < floorCentre; x++) {
			for (int y = 0; y < floorCentre - x; y++) {
				matrix[x][y] = -1;
			}
		}
	}


	/* Esempio del "corners pattern" su una matrice 9 x 9
	*	0 -> tiles senza effetto     2 -> tiles con bonus/malus
	*
	*	2 2 2 2 0 2 2 2 2
	*	2 2 2 0 0 0 2 2 2				Sottomatrice 4 x 4
	*	2 2 0 0 0 0 0 2 2					2 2 2 2
	*	2 0 0 0 0 0 0 0 2					0 2 2 2
	*	0 0 0 0 0 0 0 0 0					0 0 2 2
	*	2 0 0 0 0 0 0 0 2					0 0 0 2
	*	2 2 0 0 0 0 0 2 2
	*	2 2 2 0 0 0 2 2 2
	*	2 2 2 2 0 2 2 2 2
	*
	*/
	void CreateCornersPattern(int matrix[floorCentre][floorCentre]) {
		for (int x = 0; x < floorCentre; x++) {
			for (int y = x; y < floorCentre; y++) {
				matrix[x][y] = -1;
			}
		}
	}


	/* Esempio del "diagonal pattern" su una matrice 9 x 9
	*	0 -> tiles senza effetto     2 -> tiles con bonus/malus
	*
	*	2 2 0 0 0 0 0 2 2
	*	2 2 2 0 0 0 2 2 2				Sottomatrice 4 x 4
	*	0 2 2 2 0 2 2 2 0					0 0 2 2
	*	0 0 2 2 0 2 2 0 0					0 2 2 2
	*	0 0 0 0 0 0 0 0 0					2 2 2 0
	*	0 0 2 2 0 2 2 0 0					2 2 0 0
	*	0 2 2 2 0 2 2 2 0
	*	2 2 2 0 0 0 2 2 2
	*	2 2 0 0 0 0 0 2 2
	*/
	void CreateDiagonalPattern(int matrix[floorCentre][floorCentre]) {
		int bound = floorCentre - round((double)floorCentre / 2);

		for (int x = 0; x < bound; x++) {
			for (int y = 0; y < bound - x; y++) {
				// Riempi l'angolo in alto a sinistra della sottomatrice
				matrix[x][y] = -1;

				// Riempi l'angolo in basso a destra della sottomatrice
				matrix[floorCentre - 1 - x][floorCentre - 1 - y] = -1;
			}
		}

		// Crea il "negativo" della matrice (scambia le tiles 'effetto' con quelle 'senza effetto')
		for (int x = 0; x < floorCentre; x++) {
			for (int y = 0; y < floorCentre; y++) {
				if (matrix[x][y] == -1) {
					matrix[x][y] = -2;
				}
				else {
					matrix[x][y] = -1;
				}
			}
		}
	}


	//Ruota una matrice in senso antiorario di 90° k volte, dove k = 0,1,2,...
	void RotateMatrix(int matrix[floorCentre][floorCentre], int rotations) {
		int tempMatrix[floorCentre][floorCentre] = {};

		// il numero di rotazioni è (rotations % 4) perché ogni 4 rotazioni
		// è come se la matrice non venisse ruotata affatto
		for (int i = 0; i < rotations % 4; i++) {

			//riempie la matrice temporanea
			for (int x = 0; x < floorCentre; x++) {
				for (int y = floorCentre - 1; y >= 0; y--) {
					tempMatrix[floorCentre - 1 - y][x] = matrix[x][y];
				}
			}

			//aggiorna il contenuto della matrice originale
			for (int x = 0; x < floorCentre; x++) {
				for (int y = 0; y < floorCentre; y++) {
					matrix[x][y] = tempMatrix[x][y];
				}
			}
		}
	}




	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	TArray<UBoxComponent*> triggers;


	UFUNCTION(BlueprintCallable)
	/* Tiene traccia della morte dei nemici e quando vengono tutti sconfitti
	*  le porte della stanza vengono aperte e viene spawnato l'eventuale portale
	*/
	void UpdateRoomUponEnemyDeath() {
		UE_LOG(LogTemp, Error, TEXT("An enemy called this function!"));
		enemiesInTheRoom--;
		if (enemiesInTheRoom == 0) {
			DeleteDoors();
			if (roomContainsPortal) {
				FActorSpawnParameters spawnParams;
				FVector posOffset = FVector(0.0, 0.0, 95.0);
				GetWorld()->SpawnActor<APortal>(GetActorLocation() + posOffset, FRotator(0, 0, 0), spawnParams);
			}
		}
	}


	UFUNCTION()
	void OnTriggerOverlap(class AActor* OverlappedActor, class AActor* OtherActor) {
		FString nameTarget = "BP_MainCharacter_C_0";
		FString actorName = OtherActor->GetDebugName(OtherActor);

		//I trigger delle porte rispondono alle collisioni solo se attraversati dal player
		if (actorName == nameTarget) {
			if (!playerEnteredRoomAlready) {
				playerEnteredRoomAlready = true;
				//Se il giocatore entra per la prima volta nella stanza e dovrebbero esserci dei nemici...
				if (enemiesInTheRoom > 0) {
					// ...Spawna l'attore "EnemySpawner" che si occuperà di popolare la stanza con i nemici
					FActorSpawnParameters spawnParams;
					AEnemySpawner* enemySpawner = GetWorld()->SpawnActor<AEnemySpawner>(GetActorLocation(), FRotator(0, 0, 0), spawnParams);
					enemySpawner->SpawnEnemiesInsideRoom(floorTilesTypes, enemiesInTheRoom);

					// e infine spawna le porte
					SpawnDoors(roomID);
				}
				else { //Se il giocatore entra per la prima volta nella stanza e non ci sono dei nemici, distruggi le porte
					DeleteDoors();
					if (roomContainsPortal) {
						UE_LOG(LogTemp, Error, TEXT("You have entered the room containing the portal"));
						//Spawna il portale al centro della stanza
					}
				}
			}
		}

	}



};
