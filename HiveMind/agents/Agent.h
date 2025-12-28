#pragma once
#include "AgentState.h"
#include "../map/GridMap.h"

class Agent
{
    protected:
        Position position;
        AgentState state;

        int batteryLevel;
        int maxBatteryLevel;

        int speed;
        int consume;
        int costPerTick;
        int capacity;

    public: 
        Agent(Position startPosition);
        virtual ~Agent() = default;

        virtual void update(const GridMap& map) = 0;

        AgentState getState() const;
        Position getPosition() const;

        bool isDead() const;
};