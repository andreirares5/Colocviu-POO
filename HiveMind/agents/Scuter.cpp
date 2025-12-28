#include "Scuter.h"

Scuter :: Scuter(Position startPosition) : Agent(startPosition)
{
    maxBatteryLevel = 200;
    batteryLevel = maxBatteryLevel;

    speed = 2;
    consume = 5;
    costPerTick = 4;
    capacity = 2;
}

void Scuter :: update(const GridMap& map)
{
    if(batteryLevel <= 0)
    {
        state = AgentState :: DEAD;
        return;
    }

    batteryLevel = batteryLevel -consume;
    state = AgentState :: MOVING;
}