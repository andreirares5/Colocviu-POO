#include "Robot.h"

Robot :: Robot(Position startPosition) : Agent(startPosition)
{
    maxBatteryLevel = 300;
    batteryLevel = maxBatteryLevel;

    speed = 1;
    consume = 2;
    costPerTick = 1;
    capacity = 4;
}

void Robot :: update(const GridMap& map)
{
    if(batteryLevel <= 0)
    {
        state = AgentState :: DEAD;
        return;
    }

    batteryLevel = batteryLevel -consume;
    state = AgentState :: MOVING;
}