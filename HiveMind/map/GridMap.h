#pragma once
#include<vector>
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
        int rows;
        int columns;
        std :: vector<std :: vector<TileType>> grid;

        Position base;
        std :: vector<Position> stations;
        std :: vector<Position> clients;

    public:
        GridMap(int rows, int columns); 

        void setTtyle(int x, int y, TileType type);
        TileType getTileType(int x, int y) const;
};
