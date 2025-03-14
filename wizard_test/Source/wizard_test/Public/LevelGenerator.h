// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class WIZARD_TEST_API ALevelGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALevelGenerator();

protected:

    static const int levelSize = 5; // lunghezza (espressa in stanze) del lato del piano di gioco (dev'essere un numero dispari)
    static const int roomsLengthInTiles = 7; // lunghezza (espressa in tiles) del lato delle stanze
    static const int floorTilesWidth = 200; // lunghezza delle tiles quadrate componenti il pavimento delle stanze
    static const int wallsWidth = 100; // spessore dei muri delle stanze

    int roomsPathsMatrix[levelSize][levelSize] = { 0 }; // matrice che specifica i percorsi di stanze nel piano
    int doorsPositionMatrix[levelSize][levelSize] = { 0 }; // matrice che specifica numero e posizione delle porte nelle stanze
    TArray< TTuple<int, int> > roomsQueue; // queue per lo sviluppo dei vari percorsi nel piano
    int indexOffset = -1; // offset per posizionare correttamente le stanze nella queue
    int roomsInLongestPath = 0; // tiene traccia del numero di stanze contenute nel percorso più lungo
    int roomsInCurrentPath = 0; // tiene traccia del numero di stanze contenute nel percorso attualmente in costruzione
    int portalRoomX = 0; // ascissa della stanza che conterrà il portale per il livello successivo
    int portalRoomY = 0; // ordinata della stanza che conterrà il portale per il livello successivo


    // Called when the game starts or when spawned
    virtual void BeginPlay() override;


    void PreconfigureLevel() {
        //randomizzazione del seme per avere percorsi diversi ad ogni esecuzione
        srand(time(0));

        //riempimento della "roomsPathsMatrix" con tutti -2 (con assegnazione di 0 alla stanza centrale)
        for (int row = 0; row < levelSize; row++) {
            for (int col = 0; col < levelSize; col++) {
                roomsPathsMatrix[row][col] = -2;
            }
        }
        roomsPathsMatrix[(levelSize - 1) / 2][(levelSize - 1) / 2] = 0;

        //riempimento del 4-intorno della stanza centrale (configurazione delle stanze iniziali dei 4 percorsi)
        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            int roomX = ((levelSize - 1) / 2) - (int)round(sin(angle * PI));
            int roomY = ((levelSize - 1) / 2) + (int)round(cos(angle * PI));

            roomsPathsMatrix[roomX][roomY] = (int)(angle / 0.5) + 1;

            //Se invece le stanze del 4-intorno devono essere spawnate con una certa probabilità
            /*
            if (rand() % 2 == 1) {
                roomsPathsMatrix[roomX][roomY] = (int)(angle / 0.5) + 1;
            }
            else {
                roomsPathsMatrix[roomX][roomY] = -1;
            }
            */
        }
    }


    void BuildRoomsPath(int currentX, int currentY, int path) {

        // spawna le eventuali nuove stanze nel 4-intorno
        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            int roomX = currentX - (int)round(sin(angle * PI));
            int roomY = currentY + (int)round(cos(angle * PI));

            //controlla se l'eventuale nuova stanza non è fuori dai bordi
            if ((roomX >= 0 && roomX < levelSize) && (roomY >= 0 && roomY < levelSize)) {
                // controlla se la cella non è già occupata da un'altra stanza
                if (roomsPathsMatrix[roomX][roomY] == -2) {
                    if (rand() % 2 == 1) {
                        // controlla se la stanza nuova genera un loop
                        int neighbors = 0;
                        int neighborX, neighborY;

                        for (double theta = 0.0; theta <= 1.5; theta += 0.5) {
                            for (double increment = 0.0; increment <= 0.5; increment += 0.25) {
                                // calcolo posizione stanza "neighbor" e check "out of bounds"
                                neighborX = roomX - (int)round(sin(theta * PI + increment * PI));
                                neighborY = roomY + (int)round(cos(theta * PI + increment * PI));

                                if ((neighborX >= 0 && neighborX < levelSize) && (neighborY >= 0 && neighborY < levelSize)) {
                                    if (roomsPathsMatrix[neighborX][neighborY] == path) {
                                        neighbors++;
                                    }
                                }
                            }
                            // dopo aver controllato i 3 "neighbors", se sono tutti pieni
                            // non spawnare la stanza ed esci fuori dal ciclo
                            if (neighbors == 3) {
                                roomsPathsMatrix[roomX][roomY] = -1;
                                break;
                            }

                            // reset del contatore per i "neighbors" successivi
                            neighbors = 0;
                        }
                        // controllati tutti e otto i "neighbors" (no rischio di loops) spawna la stanza
                        if (neighbors != 3) {
                            roomsPathsMatrix[roomX][roomY] = path;
                            indexOffset++;
                            roomsQueue.Insert(TTuple<int, int>(roomX, roomY), roomsQueue.Num() - 1 - indexOffset);
                        }
                    }
                    else {
                        roomsPathsMatrix[roomX][roomY] = -1;
                    }
                }
            }
        }
    }


    void DetermineDoorsPosition() {
        //assegnazione dei valori della "doorsPositionMatrix" (determinazione di numero e disposizione delle porte per ogni stanza)
        int neighborX, neighborY, exponent, path;

        for (int roomX = 0; roomX < levelSize; roomX++) {
            for (int roomY = 0; roomY < levelSize; roomY++) {
                exponent = 0;

                if (roomsPathsMatrix[roomX][roomY] >= 0) {
                    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
                        neighborX = roomX - (int)round(sin(angle * PI));
                        neighborY = roomY + (int)round(cos(angle * PI));

                        if ((neighborX >= 0 && neighborX < levelSize) && (neighborY >= 0 && neighborY < levelSize)) {
                            if ((roomsPathsMatrix[neighborX][neighborY] == roomsPathsMatrix[roomX][roomY]) or (roomsPathsMatrix[neighborX][neighborY] == 0)) {
                                doorsPositionMatrix[roomX][roomY] += pow(2, exponent);
                            }
                        }

                        exponent++;
                    }
                }
                else {
                    doorsPositionMatrix[roomX][roomY] = -1;
                }
            }
        }

        exponent = 0;
        path = 1;

        for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
            neighborX = (levelSize - 1) / 2 - (int)round(sin(angle * PI));
            neighborY = (levelSize - 1) / 2 + (int)round(cos(angle * PI));

            if (roomsPathsMatrix[neighborX][neighborY] == path) {
                doorsPositionMatrix[(levelSize - 1) / 2][(levelSize - 1) / 2] += pow(2, exponent);
            }

            exponent++;
            path++;
        }
    }

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;


    UPROPERTY()
    USceneComponent* Root;

};