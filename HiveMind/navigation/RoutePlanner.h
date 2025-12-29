#pragma once
#include "Path.h"
#include "../map/GridMap.h"

class RoutePlanner
{
    public:
        virtual ~RoutePlanner() = default;

        virtual Path planPath(const GridMap& map, const Position& start, const Position& goal, bool ignoreWalls) const = 0;
};