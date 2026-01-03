#include "HiveMind.h"

void HiveMind :: decide (std :: vector<std :: unique_ptr<Agent>> &agents, const GridMap &map, const RoutePlanner &planner)
{
    if(map.getClientsPositions().empty())
        return;

    Agent *agent = freeAgent(agents);
    if(agent == nullptr)
        return;

    Position start = agent -> getPosition();
    Position goal = map.getClientsPositions().front();

    Path path = planner.planPath(map, start, goal, false);

    if(path.isEmpty())
        return;

    if(!hasEnoughEnergy(*agent, path))
        return;
}