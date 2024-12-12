/******************************************************************************

Il seguente algoritmo permette di creare 4 percorsi random, continui e senza loops di stanze.
Il risultato è costituito dalla matrice "valuesMatrix" la quale contiene informazioni su:
    - come sono disposte le stanze nel piano
    - quante porte ogni stanza deve avere
    - la disposizione delle porte in ogni stanza

*******************************************************************************/

#include <iostream>
#include <cmath>
#include <list>
#include <tuple>

using namespace std;

const int floorSize = 5; //dev'essere un numero dispari almeno pari a 3
const double PI = 3.14159265359;
int pathsMatrix[floorSize][floorSize] = { 0 };
int valuesMatrix[floorSize][floorSize] = { 0 };
list< tuple<int, int> > roomsQueue;
list< tuple<int, int> >::iterator itr;

void PreconfigureFloor();
void BuildPath(int currentX, int currentY, int path);
void SetupDoors();

int main()
{
    // configurazione di un piano di gioco in cui le stanze percorribili inizialmente sono al più cinque:
    // quella centrale e le quattro stanze intorno ad esso
    PreconfigureFloor();


    //generazione dei max quattro percorsi indipendenti che partono dalle quattro stanze intorno a quella centrale
    int path = 1;

    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
        int roomX = ((floorSize - 1) / 2) - (int)sin(angle * PI);
        int roomY = ((floorSize - 1) / 2) + (int)cos(angle * PI);

        if (pathsMatrix[roomX][roomY] != -1) {
            roomsQueue.push_back(tuple<int, int>(roomX, roomY));
            itr = roomsQueue.end();
            while (roomsQueue.size() != 0) {
                BuildPath(get<0>(roomsQueue.back()), get<1>(roomsQueue.back()), path);
                roomsQueue.pop_back();
                itr = roomsQueue.end();
            }
        }
        path++;
    }

    //vengono determinati per ogni stanza il numero e la disposizione delle porte
    SetupDoors();
    
    
    cout << "Paths matrix" << endl;
    for (int row = 0; row < floorSize; row++) {
        for (int col = 0; col < floorSize; col++) {
            cout << pathsMatrix[row][col] << ' ';
        }
        cout << endl;
    }

    cout << endl << "Values matrix" << endl;
    for (int row = 0; row < floorSize; row++) {
        for (int col = 0; col < floorSize; col++) {
            cout << valuesMatrix[row][col] << ' ';
        }
        cout << endl;
    }
    

    return 0;
}

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

    //riempimento del 4-intorno della stanza centrale (configurazione delle stanze iniziali dei 4 paths)
    for (double angle = 0.0; angle <= 1.5; angle += 0.5) {
        int roomX = ((floorSize - 1) / 2) - (int)sin(angle * PI);
        int roomY = ((floorSize - 1) / 2) + (int)cos(angle * PI);

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
        int roomX = currentX - (int)sin(angle * PI);
        int roomY = currentY + (int)cos(angle * PI);

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
                            neighborX = roomX - round(sin(theta * PI + increment * PI));
                            neighborY = roomY + round(cos(theta * PI + increment * PI));

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
                        itr--;
                        roomsQueue.insert( itr, tuple<int,int>(roomX,roomY) );
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
                    neighborX = roomX - (int)sin(angle * PI);
                    neighborY = roomY + (int)cos(angle * PI);

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
        neighborX = (floorSize - 1) / 2 - (int)sin(angle * PI);
        neighborY = (floorSize - 1) / 2 + (int)cos(angle * PI);

        if (pathsMatrix[neighborX][neighborY] == path) {
            valuesMatrix[(floorSize - 1) / 2][(floorSize - 1) / 2] += pow(2, exponent);
        }

        exponent++;
        path++;
    }
}