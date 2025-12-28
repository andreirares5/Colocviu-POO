#include "Drona.h"

Drona :: Drona(Position startPosition) : Agent(startPosition) 
{
    maxBatteryLevel = 100;
    batteryLevel = maxBatteryLevel;

    speed = 3;
    consume = 10;
    costPerTick = 15;
    capacity = 1;
}

void Drona :: update(const GridMap& map)
{
    if(batteryLevel <= 0)
    {
        state = AgentState :: DEAD;
        return;
    }

    batteryLevel = batteryLevel -consume;
    state = AgentState :: MOVING;
}