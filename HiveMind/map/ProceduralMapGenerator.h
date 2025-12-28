#pragma once
#include "IMapGenerator.h"

class ProceduralMapGenerator : public IMapGenerator
{
    private:
        int rows;
        int columns;
        int maxStations;
        int clientsCount;
 
    public:
        ProceduralMapGenerator(int rows, int columns, int maxStations, int clientsCount);
        std :: unique_ptr<GridMap> generate() override;
};