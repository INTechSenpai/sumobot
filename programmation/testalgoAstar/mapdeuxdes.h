#ifndef MAPDEUXDES_H
#define MAPDEUXDES_H
#define N 10
#include <utility>


class Mapdeuxdes
{
public:
    Mapdeuxdes();
    void afficherMap();
    void marquerMap(int i, int j);
    bool estUnObstacle(int i, int j);
    std::pair<int, int> getDepart();
    std::pair<int, int> getArrive();

private:
    char *lamap[N][N];
    int xdepart;
    int ydepart;
    int xarrive;
    int yarrive;

};

#endif // MAPDEUXDES_H
