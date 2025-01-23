// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StringOutputter.generated.h"

UCLASS()
class PROVA_ESAME_API AStringOutputter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStringOutputter();

protected:

	static const int floorSize = 5; // numero di stanze per piano (dev'essere un numero dispari
    static const int floorWidth = 700; // larghezza del pavimento quadrato delle stanze
    static const int wallWidth = 50; // spessore dei muri delle stanze

	int pathsMatrix[floorSize][floorSize] = { 0 }; // matrice che specifica i percorsi di stanze nel piano
	int valuesMatrix[floorSize][floorSize] = { 0 }; // matrice che specifica numero e posizione delle porte nelle stanze
	TArray< TTuple<int, int> > roomsQueue; // queue per lo sviluppo dei vari percorsi nel piano
    int indexOffset = -1; // offset per posizionare correttamente le stanze nella queue



    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


	void PreconfigureFloor() {
        //randomizzazione del seme per avere percorsi diversi ad ogni esecuzione
        srand(time(0));

        //riempimento della "pathsMatrix" con tutti -2 (con assegnazione di 0 alla stanza centrale)
        for (int row = 0; row < floorSize; row++) {
            for (int col = 0; col < floorSize; col++) {
                pathsMatrix[row][col] = -2;
            }
        }
        pathsMatrix[(floorSize - 1) / 2][(floorSize - 1) / 2] = 0;

        //riempimento del 4-intorno della stanza centrale (configurazione delle stanze iniziali dei 4 percorsi)
        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            int roomX = ((floorSize - 1) / 2) - (int)round(sin(angle * PI));
            int roomY = ((floorSize - 1) / 2) + (int)round(cos(angle * PI));

            if (rand() % 2 == 1) {
                pathsMatrix[roomX][roomY] = (int)(angle / 0.5) + 1;
            }
            else {
                pathsMatrix[roomX][roomY] = -1;
            }
        }
	}

    
    void BuildPath(int currentX, int currentY, int path) {

        // spawna le eventuali nuove stanze nel 4-intorno
        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            int roomX = currentX - (int)round(sin(angle * PI));
            int roomY = currentY + (int)round(cos(angle * PI));

            //controlla se l'eventuale nuova stanza non è fuori dai bordi
            if ((roomX >= 0 && roomX < floorSize) && (roomY >= 0 && roomY < floorSize)) {
                // controlla se la cella non è già occupata da un'altra stanza
                if (pathsMatrix[roomX][roomY] == -2) {
                    if (rand() % 2 == 1) {
                        // controlla se la stanza nuova genera un loop
                        int neighbors = 0;
                        int neighborX, neighborY;

                        for (double theta = 0.0; theta <= 1.5; theta += 0.5) {
                            for (double increment = 0.0; increment <= 0.5; increment += 0.25) {
                                // calcolo posizione stanza "neighbor" e check "out of bounds"
                                neighborX = roomX - (int)round(sin(theta * PI + increment * PI));
                                neighborY = roomY + (int)round(cos(theta * PI + increment * PI));

                                if ((neighborX >= 0 && neighborX < floorSize) && (neighborY >= 0 && neighborY < floorSize)) {
                                    if (pathsMatrix[neighborX][neighborY] == path) {
                                        neighbors++;
                                    }
                                }
                            }
                            // dopo aver controllato i 3 "neighbors", se sono tutti pieni
                            // non spawnare la stanza ed esci fuori dal ciclo
                            if (neighbors == 3) {
                                pathsMatrix[roomX][roomY] = -1;
                                break;
                            }

                            // reset del contatore per i "neighbors" successivi
                            neighbors = 0;
                        }
                        // controllati tutti e otto i "neighbors" (no rischio di loops) spawna la stanza
                        if (neighbors != 3) {
                            pathsMatrix[roomX][roomY] = path;
                            indexOffset++;
                            roomsQueue.Insert(TTuple<int, int>(roomX, roomY), roomsQueue.Num() - 1 - indexOffset);
                        }
                    }
                    else {
                        pathsMatrix[roomX][roomY] = -1;
                    }
                }
            }
        }
    }

    void SetupDoors() {
        //assegnazione dei valori della "valuesMatrix" (determinazione di numero e disposizione delle porte per ogni stanza)
        int neighborX, neighborY, exponent, path;

        for (int roomX = 0; roomX < floorSize; roomX++) {
            for (int roomY = 0; roomY < floorSize; roomY++) {
                exponent = 0;

                if (pathsMatrix[roomX][roomY] >= 0) {
                    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
                        neighborX = roomX - (int)round(sin(angle * PI));
                        neighborY = roomY + (int)round(cos(angle * PI));

                        if ((neighborX >= 0 && neighborX < floorSize) && (neighborY >= 0 && neighborY < floorSize)) {
                            if ((pathsMatrix[neighborX][neighborY] == pathsMatrix[roomX][roomY]) or (pathsMatrix[neighborX][neighborY] == 0)) {
                                valuesMatrix[roomX][roomY] += pow(2, exponent);
                            }
                        }

                        exponent++;
                    }
                }
                else {
                    valuesMatrix[roomX][roomY] = -1;
                }
            }
        }

        exponent = 0;
        path = 1;

        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            neighborX = (floorSize - 1) / 2 - (int)round(sin(angle * PI));
            neighborY = (floorSize - 1) / 2 + (int)round(cos(angle * PI));

            if (pathsMatrix[neighborX][neighborY] == path) {
                valuesMatrix[(floorSize - 1) / 2][(floorSize - 1) / 2] += pow(2, exponent);
            }

            exponent++;
            path++;
        }
    }
    

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
