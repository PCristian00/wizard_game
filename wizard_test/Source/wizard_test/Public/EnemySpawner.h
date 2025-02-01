// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "chrono"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Spawna un numero random di nemici di tipo melee e/o ranged
	int SpawnEnemiesInsideRoom(int floorMatrix[roomsLengthInTiles][roomsLengthInTiles]) {
		int x, y, enemiesToSpawn, enemiesSpawned, enemyPosX, enemyPosY, distanceX, distanceY;
		FActorSpawnParameters spawnParams;
		FVector spawnPos = GetActorLocation();
		int enemyPosOffset = (roomsLengthInTiles - 1) / 2;
		int floorCentre = enemyPosOffset;

		// Scegli randomicamente quanti nemici spawnare MIN: 0   MAX: maxEnemiesInsideOneRoom
		srand(static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		enemiesToSpawn = rand() % (maxEnemiesInsideOneRoom + 1);
		enemiesSpawned = 0;

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
					
					// Spawna randomicamente un nemico di tipo "melee" o "ranged"
					if (rand() % 2 == 0) {
						GetWorld()->SpawnActor<ARangedEnemy>(spawnPos + FVector(enemyPosX, enemyPosY, 100), FRotator(0, 0, 0), spawnParams);
					}
					else {
						GetWorld()->SpawnActor<AMeleeEnemy>(spawnPos + FVector(enemyPosX, enemyPosY, 100), FRotator(0, 0, 0), spawnParams);
					}

					// Setta l'elemento della matrice a zero per indicare che sulla tile è spawnato un nemico
					floorMatrix[x][y] = 0;
					enemiesToSpawn--;
					enemiesSpawned++;
				}
			}
		}

		return enemiesSpawned;
	}

	UPROPERTY()
	USceneComponent* Root;
};
