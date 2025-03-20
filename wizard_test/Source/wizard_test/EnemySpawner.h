// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "chrono"
#include "GameFramework/Character.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class WIZARD_TEST_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static const int roomsLengthInTiles = 7; // lunghezza (espressa in tiles) del lato delle stanze
	static const int floorTilesWidth = 200; // lunghezza delle tiles quadrate componenti il pavimento delle stanze
	int maxEnemiesInsideOneRoom = 4;
	TSubclassOf<ACharacter> iceSpider, fireSpider, lightSpider, iceSnake, lightSnake, boar; // nemici melee
	TSubclassOf<ACharacter> iceBat, fireBat, lightBat, iceWasp, fireWasp, lightWasp; // nemici ranged

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Spawna un numero random di nemici di tipo melee e/o ranged
	void SpawnEnemiesInsideRoom(int floorMatrix[roomsLengthInTiles][roomsLengthInTiles], int enemiesToSpawn) {
		int x, y, enemyPosX, enemyPosY, distanceX, distanceY;
		FActorSpawnParameters spawnParams;
		FVector spawnPos = GetActorLocation();
		int enemyPosOffset = (roomsLengthInTiles - 1) / 2;
		int floorCentre = enemyPosOffset;


		while (enemiesToSpawn > 0) {
			// Seleziona a caso una delle tiles che compongono il pavimento della stanza
			srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			x = rand() % roomsLengthInTiles;
			y = rand() % roomsLengthInTiles;

			/* Un nemico viene spawnato su tale tile a patto che:
			*	I - la tile sia una tile senza effetto (bonus/malus)
			*	II - la tile non sia di fronte ad una porta
			*	III - sulla tile non è già stato spawnato un nemico
			*/
			if (floorMatrix[x][y] == -2) {
				enemyPosX = (y - enemyPosOffset) * floorTilesWidth;
				enemyPosY = (x - enemyPosOffset) * floorTilesWidth;

				distanceX = x - floorCentre;
				distanceY = y - floorCentre;

				/* Dato che la stanza è un quadrato di lato dispari e dato che le porte sono disposte
				*  al centro dei lati di tale quadrato, si ha sempre che una delle coordinate X,Y di una tile vicino
				*  ad una porta è pari a "floorCentre". Di conseguenza una tra "distanceX" e "distanceY" sarà zero e
				*  quindi il loro prodotto farà zero.
				*
				*  O O X O O
				*  O O O O O
				*  X O O O X      dove 'l' (in questo caso 2) è "floorCentre"
				*  O O O O O
				*  O O X O O
				*
				*      < - >
				*		 l
				*/
				if (distanceX * distanceY != 0) {
					switch (rand() % 12) {
						case 0:
							GetWorld()->SpawnActor<ACharacter>(iceSpider, spawnPos + FVector(enemyPosX, enemyPosY, 200), FRotator(0, 0, 0), spawnParams);
							break;
						case 1:
							GetWorld()->SpawnActor<ACharacter>(fireSpider, spawnPos + FVector(enemyPosX, enemyPosY, 200), FRotator(0, 0, 0), spawnParams);
							break;
						case 2:
							GetWorld()->SpawnActor<ACharacter>(lightSpider, spawnPos + FVector(enemyPosX, enemyPosY, 200), FRotator(0, 0, 0), spawnParams);
							break;
						case 3:
							GetWorld()->SpawnActor<ACharacter>(iceSnake, spawnPos + FVector(enemyPosX, enemyPosY, 220), FRotator(0, 0, 0), spawnParams);
							break;
						case 4:
							GetWorld()->SpawnActor<ACharacter>(lightSnake, spawnPos + FVector(enemyPosX, enemyPosY, 220), FRotator(0, 0, 0), spawnParams);
							break;
						case 5:
							GetWorld()->SpawnActor<ACharacter>(boar, spawnPos + FVector(enemyPosX, enemyPosY, 300), FRotator(0, 0, 0), spawnParams);
							break;
						case 6:
							GetWorld()->SpawnActor<ACharacter>(iceBat, spawnPos + FVector(enemyPosX, enemyPosY, 240), FRotator(0, 0, 0), spawnParams);
							break;
						case 7:
							GetWorld()->SpawnActor<ACharacter>(fireBat, spawnPos + FVector(enemyPosX, enemyPosY, 240), FRotator(0, 0, 0), spawnParams);
							break;
						case 8:
							GetWorld()->SpawnActor<ACharacter>(lightBat, spawnPos + FVector(enemyPosX, enemyPosY, 240), FRotator(0, 0, 0), spawnParams);
							break;
						case 9:
							GetWorld()->SpawnActor<ACharacter>(iceWasp, spawnPos + FVector(enemyPosX, enemyPosY, 280), FRotator(0, 0, 0), spawnParams);
							break;
						case 10:
							GetWorld()->SpawnActor<ACharacter>(fireWasp, spawnPos + FVector(enemyPosX, enemyPosY, 280), FRotator(0, 0, 0), spawnParams);
							break;
						case 11:
							GetWorld()->SpawnActor<ACharacter>(lightWasp, spawnPos + FVector(enemyPosX, enemyPosY, 280), FRotator(0, 0, 0), spawnParams);
							break;
					}

					// Setta l'elemento della matrice a zero per indicare che sulla tile è spawnato un nemico
					floorMatrix[x][y] = 0;
					enemiesToSpawn--;
				}
			}
		}

	}

	UPROPERTY()
	USceneComponent* Root;
};
