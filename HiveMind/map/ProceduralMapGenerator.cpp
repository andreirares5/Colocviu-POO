#include "ProceduralMapGenerator.h"
#include<ctime>
#include<cstdlib>

ProceduralMapGenerator :: ProceduralMapGenerator(int rows, int columns, int max, int count) 
    : rows(rows), columns(columns), maxStations(max), clientsCount(count)
{
    std :: srand(static_cast<unsigned int>(std :: time(nullptr)));
}

std :: unique_ptr<GridMap> ProceduralMapGenerator :: generate()
{
    auto map = std :: make_unique<GridMap>(rows, columns);

    // aici plasez zidurile random pe harta cu o repartitie de 25%
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            int probability = std :: rand() % 100;
            
            if(probability < 25)
            {
                map ->setTile(i , j, TileType :: Wall);
            }
        }
    }

    //plasez baza pe harta in mod aleator
    int bx, by;

    do
    {
        bx = std :: rand() % rows;
        by = std :: rand() % columns;
    } while (map -> getTileType(bx, by) == TileType :: Wall);
    
    map -> setTile(bx, by, TileType :: Base);


    //aici voi plasa statiile
    int stations = 0;
    while(stations < maxStations)
    {
        int x = std :: rand() % rows;
        int y = std :: rand() % columns;

        if(map -> getTileType(x, y) == TileType :: Road)
        {
            map -> setTile(x, y, TileType :: Station);
            stations ++;
        }
    }

    //aici voi plasa clientii/destinatiile
    int clients = 0;
    while(clients < clientsCount)
    {
        int x = std :: rand() % rows;
        int y = std :: rand() % columns;

        if(map -> getTileType(x, y) == TileType :: Road)
        {
            map -> setTile(x, y, TileType :: Client);
            clients ++;
        }
    }

    return map;
}
