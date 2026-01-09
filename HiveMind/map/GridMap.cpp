#include "GridMap.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>

GridMap :: GridMap(int rows, int cols) : rows(rows), columns(cols)
{
    if (rows <= 0 || cols <= 0) //verific dimensiunile hartii
        throw std :: invalid_argument("Dimensiuni invalide");

    grid.resize(rows); //initializez harta doar cu celule de tip road
    for (int i = 0; i < rows; i++)
        grid[i].resize(cols, TileType :: Road);
}

void GridMap::setTile(int x, int y, TileType type)
{
    if (!isInside(x, y)) //verific daca pozitia se afla pe harta
        throw std :: out_of_range("Pozitia nu se afla pe harta");

    grid[x][y] = type;
    Position position{x, y};

    if (type == TileType :: Base) //daca celula este baza, atunci salvez pozitia ei(este si unica)
        base = position;
    else if (type == TileType :: Station) //daca este statie, o adaug in lista statiilor  daca nu exista deja
    {
        if (std :: find(stations.begin(), stations.end(), position) == stations.end())
            stations.push_back(position);
    }
    else if (type == TileType :: Client) // daca este client, il adaug in lista clientilor daca nu exista deja
    {
        if (std :: find(clients.begin(), clients.end(), position) == clients.end())
            clients.push_back(position);
    }
}

TileType GridMap :: getTileType(int x, int y) const
{
    if (!isInside(x, y))
        throw std :: out_of_range("Pozitia nu se afla pe harta");

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

const std::vector<Position>& GridMap :: getStationsPositions() const
{
    return stations;
}

const std::vector<Position>& GridMap :: getClientsPositions() const
{
    return clients;
}

bool GridMap :: isInside(int x, int y) const
{
    return x >= 0 && y >= 0 && x < rows && y < columns;
}

bool GridMap :: isWalkable(int x, int y, bool ignoreWalls) const //verifica daca pe pozitia {x,y} se poate merge
{
    if (!isInside(x, y)) 
        return false;

    if (grid[x][y] == TileType :: Wall && !ignoreWalls) 
        return false;

    return true;
}

static char tileTypeToChar(TileType type) //auxiliar am creat aceasta functie care imi converteste tipul celulei in caracter pentru a o salva ulterior intr-un fisier
{
    switch (type)
    {
        case TileType :: Road:    return '.';
        case TileType :: Wall:    return '#';
        case TileType :: Base:    return 'B';
        case TileType :: Station: return 'S';
        case TileType :: Client:  return 'D';
        default:                return '?';
    }
}

void GridMap :: printMap(const std::string& filename) const //afisez harta in fisier text
{
    std :: ofstream out(filename); 
    if (!out.is_open()) return;

    for (int i = 0; i < rows; i++) //scriu harta linie cu linie
    {
        for (int j = 0; j < columns; j++)
            out << tileTypeToChar(grid[i][j]);
        out << '\n';
    }
}
