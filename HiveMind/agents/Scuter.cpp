#include "Scuter.h"
#include <algorithm>

Scuter :: Scuter(Position startPosition)
    : Agent(startPosition, BATTERY, CONSUMPTION, SPEED, COST, CAPACITY)
{}

void Scuter :: update(const GridMap& map)
{
    if (state == AgentState :: DEAD)
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
        state = AgentState :: DEAD;
        return;
    }

    if (!(position == destination))
    {
        state = AgentState :: MOVING;

        for (int step = 0; step < speed; ++step)
        {
            if (position == destination)
                break;

            Position next = position;

            if (position.x != destination.x)
                next.x += (destination.x > position.x ? 1 : -1);
            else if (position.y != destination.y)
                next.y += (destination.y > position.y ? 1 : -1);

            if (map.isInside(next.x, next.y) &&
                map.isWalkable(next.x, next.y, false))
            {
                position = next;
            }
            else
            {
                next = position;
                if (position.y != destination.y)
                    next.y += (destination.y > position.y ? 1 : -1);
                else if (position.x != destination.x)
                    next.x += (destination.x > position.x ? 1 : -1);

                if (map.isInside(next.x, next.y) &&
                    map.isWalkable(next.x, next.y, false))
                {
                    position = next;
                }
                else
                {
                    break; 
                }
            }
        }

        consumeBattery();
        return;
    }

    if (hasAssignedPackage())
    {
        completeDelivery();
    }

    state = AgentState::IDLE;
}