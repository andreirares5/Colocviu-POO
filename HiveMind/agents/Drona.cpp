#include "Drona.h"
#include <algorithm>

Drona::Drona(Position startPosition)
    : Agent(startPosition, BATTERY, CONSUMPTION, SPEED, COST, CAPACITY)
{}

void Drona::update(const GridMap& map)
{
    if (state == AgentState :: DEAD)
        return;

    TileType tile = map.getTileType(position.x, position.y);

    if ((tile == TileType::Station || tile == TileType::Base) &&
        batteryLevel < maxBatteryLevel)
    {
        recharge();
        state = AgentState::CHARGING;
        return;
    }

    if (!hasPackage)
    {
        state = AgentState :: IDLE;
        return;
    }

    if (position == destination)
    {
        completeDelivery();
        return;
    }

    if (path.empty())
    {
        hasPackage = false;
        state = AgentState :: IDLE;
        return;
    }

    state = AgentState :: MOVING;

    int steps = 0;


    while (steps < speed && !path.empty() && state != AgentState :: DEAD)
    {
        Position nextPos = path.front();
        path.erase(path.begin());

        if (!map.isInside(nextPos.x, nextPos.y))
        {
            hasPackage = false;
            state = AgentState :: IDLE;
            return;
        }

    position = nextPos;
    consumeBattery();

    
    if (position == destination)
    {
        completeDelivery();
        return;
    }

    steps++;
}
    if (state == AgentState :: DEAD)
        return;
}

