#pragma once
#include <vector>
#include "Tile.h"

struct Position
{
    int x;
    int y;

    bool operator ==(const Position& other) const
    {
        return x == other.x && y == other.y;
    }
};

class GridMap
{
    private:
        int rows; //M
        int columns; //N
        std :: vector<std :: vector<TileType>> grid;

        Position base;
        std :: vector<Position> stations;
        std :: vector<Position> clients;

    public:
        GridMap(int rows, int columns); 

        void setTile(int x, int y, TileType type);
        TileType getTileType(int x, int y) const;

        int getRows() const;
        int getColumns() const;

        Position getBasePosition() const;
        const std :: vector<Position>& getStationsPositions() const;
        const std :: vector<Position>& getClientsPositions() const;

        bool isInside(int x, int y) const;
        bool isWalkable(int x, int y, bool ignoreWalls) const;
};
