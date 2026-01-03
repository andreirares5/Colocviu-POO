#include "Engine.h"
#include <iostream>

Engine :: Engine(GridMap& map, HiveMind& hivemind, RoutePlanner& planner, std :: vector<std :: unique_ptr<Agent>>& agents) 
    : map(map), hivemind(hivemind), planner(planner), agents(agents), profit(profit), tickCount(tickCount) {}

void Engine :: run(int ticks)
{
    for(int i = 0; i < ticks; ++i)
    {
        tick();
    }
}

void Engine :: tick()
{
    ++ tickCount;

    hivemind.decide(agents, map, planner);    

    for( auto& agent: agents)
    {
        agent -> update(map);
    }
}

int Engine :: getProfit() const
{
    return profit;
}