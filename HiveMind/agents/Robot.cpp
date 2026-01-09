#include "Robot.h"
#include "../navigation/BFSRoute.h"
#include <algorithm>

Robot::Robot(Position startPosition)
    : Agent(startPosition, BATTERY, CONSUMPTION, SPEED, COST, CAPACITY)
{}

void Robot :: update(const GridMap& map)
{
    if (state == AgentState::DEAD)
        return;

    TileType tile = map.getTileType(position.x, position.y);
    if ((tile == TileType :: Base || tile == TileType :: Station) &&
        batteryLevel < maxBatteryLevel)
    {
        recharge();
        state = AgentState::CHARGING;
        return;
    }

    if (batteryLevel <= 0)
    {
        state = AgentState:: DEAD;
        return;
    }

    if (!(position == destination))
    {
        state = AgentState :: MOVING;

        if (path.empty())
        {
            BFSRoute planner;
            path = planner.planPath(map, position, destination, false).nodes;
        }

        if (path.size() > 1)
        {
            position = path[1];
            path.erase(path.begin()); 
        }

        consumeBattery();
        return;
    }

    if (hasAssignedPackage())
    {
        completeDelivery();
    }

    path.clear();
    state = AgentState::IDLE;
}