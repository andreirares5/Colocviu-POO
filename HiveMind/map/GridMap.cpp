#include "GridMap.h"
#include <stdexcept>

GridMap :: GridMap(int rows, int cols) : rows(rows), columns(cols) 
{
    if(rows <= 0 || cols <= 0)
        throw std :: invalid_argument("Invalid map decisions");

    grid.resize(rows);
    for(int i = 0; i < rows; i++)
    {
        grid[i].resize(cols, TileType :: Road);
    }
}

void GridMap :: setTile(int x, int y, TileType type)
{
    if(!isInside(x, y))
        throw std :: out_of_range("Position outside map");

    grid[x][y] = type;

    Position position {x, y};

    if(type == TileType :: Base)
    {
        base = position;
    }

    else if (type == TileType :: Station)
    {
        stations.push_back(position);
    }

    else if(type == TileType :: Client)
    {
        clients.push_back(position);
    }
}

TileType GridMap :: getTileType(int x, int y) const
{
    if(!isInside(x, y))
        throw std :: out_of_range("Invalid position");

    return grid[x][y];
}

int GridMap :: getRows() const
{
    return rows;
}

int GridMap :: getColumns() const
{
    return columns;
}

Position GridMap :: getBasePosition() const
{
    return base;
}

const std :: vector<Position>& GridMap :: getStationsPositions() const
{
    return stations;
}

const std :: vector<Position>& GridMap :: getClientsPositions() const
{
    return clients;
}

bool GridMap :: isInside(int x, int y) const
{
    if( x < 0 || y < 0)
        return false;

    if( x >= rows || y >= columns)
        return false;

    return true;
}

bool GridMap :: isWalkable(int x, int y, bool ignoreWalls) const
{
    if(!isInside(x, y))
        return false;

    TileType tile = grid[x][y];

    if(tile == TileType :: Wall && !ignoreWalls)
        return false;

    return true;
}