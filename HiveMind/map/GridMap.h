#pragma once
#include <vector>
#include <string>
#include "Tile.h"

struct Position //structura care va stoca pozitiile de forma {x,y}
{
    int x;
    int y;

    bool operator ==(const Position& other) const //definirea operatorului de egalitate pentru doua puncte de pe harta
    {
        return x == other.x && y == other.y;
    }
};

class GridMap
{
    private: 
        int rows; //M
        int columns; //N
        std :: vector<std :: vector<TileType>> grid; //matrice care va stoca fiecare tip de celula

        Position base; //pozitia bazei
        std :: vector<Position> stations; //pozitia statiilor
        std :: vector<Position> clients; //pozitia clientilor
    
    public:
        GridMap(int rows, int columns); //constructor

        void setTile(int x, int y, TileType type); //setez tipul celulei
        TileType getTileType(int x, int y) const; //afisez tipul celului

        int getRows() const;
        int getColumns() const;

        Position getBasePosition() const; //returnez pozitia bazei
        const std :: vector<Position>& getStationsPositions() const; //returnez pozitiile statiilor(am facut ca o referinta constanta ca sa evit copierea)
        const std :: vector<Position>& getClientsPositions() const; //returnez pozitiile clientilor

        bool isInside(int x, int y) const; //verific ca punctul de coordonate {x,y} sa fie in interiorul hartii
        bool isWalkable(int x, int y, bool ignoreWalls) const; // verifica o pozitie sa fie accesibila

        void printMap(const std :: string& filename) const; //afisez harta intr-un fisier text(nu se cere, dar mi s-a parut util)
};
