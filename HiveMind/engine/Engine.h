#pragma once
#include <vector>
#include <memory>

#include "../agents/Agent.h"
#include "../map/GridMap.h"
#include "../algorithm/HiveMind.h"
#include "../navigation/RoutePlanner.h"

class Engine
{
    private:
        GridMap& map;
        HiveMind& hivemind;
        RoutePlanner& planner;
        std :: vector<std :: unique_ptr<Agent>>& agents;

        int profit = 0;
        int tickCount = 0;

    public:
        Engine(GridMap& map, HiveMind& hivemind, RoutePlanner& planner, std :: vector<std :: unique_ptr<Agent>>& agents);

        void run(int ticks);

        void tick();

        int getProfit() const;
};