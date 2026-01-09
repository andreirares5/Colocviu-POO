#pragma once
#include "RoutePlanner.h"

class BFSRoute : public RoutePlanner 
{
    public:
        BFSRoute() = default;
        ~BFSRoute() override = default;
    
        Path planPath(const GridMap& map, const Position& start, const Position& goal, bool ignoreWalls) const override;
};