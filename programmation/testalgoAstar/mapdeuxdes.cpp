#include "mapdeuxdes.h"
#include <iostream>


/*
 0 : rien
 D : position depart
 A : position arrivée
 x : obstacle

0 0 0 0 0 0 0 0 0 0
D X 0 0 0 0 0 0 0 0
0 X 0 0 0 0 0 0 0 0
0 X 0 0 0 0 0 0 0 0
0 X 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 A

* = chemin
*/


Mapdeuxdes::Mapdeuxdes()
{
    xdepart = 0;
    ydepart = 1;

    xarrive = N-1;
    yarrive = N-1;

    for (int i=0;i<N;i++) {
        for (int j=0;j<N;j++) {
            lamap[i][j] = "0";
        }
    }
    lamap[xdepart][ydepart] = "D";
    lamap[xarrive][yarrive] = "A";

    for(int i=1;i<5;i++)
        lamap[1][i] = "X";

    for(int i=1;i<N;i++)
        lamap[5][i] = "X";

    for(int i=0;i<N-1;i++)
        lamap[3][i] = "X";

}

void Mapdeuxdes::afficherMap() {
    for (int j=0;j<N;j++) {
        for (int i=0;i<N;i++) {
            std::cout << lamap[i][j];
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Mapdeuxdes::marquerMap(int i, int j) {
    lamap[i][j] = "*";
}

bool Mapdeuxdes::estUnObstacle(int i, int j) {
    return (lamap[i][j] == "X");
}

std::pair<int, int> Mapdeuxdes::getDepart() {
    return std::make_pair(xdepart, ydepart);
}

std::pair<int, int> Mapdeuxdes::getArrive() {
    return std::make_pair(xarrive, yarrive);
}
