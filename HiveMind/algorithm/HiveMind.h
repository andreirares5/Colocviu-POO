#pragma once
#include <vector>
#include <memory>

#include "../agents/Agent.h"
#include "../map/GridMap.h"
#include "../navigation/RoutePlanner.h"

class HiveMind
{
    private:
        Agent* freeAgent(std :: vector<std :: unique_ptr<Agent>> &agents);
        bool hasEnoughEnergy(const Agent &agent, const Path& path) const;
        
    public:
        HiveMind() = default;

        void decide(std :: vector<std :: unique_ptr<Agent>> &agents, const GridMap &map,const RoutePlanner &planner);
};