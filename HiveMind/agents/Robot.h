#pragma once
#include "Agent.h"

class Robot : public Agent
{
    private:
        static const int BATTERY = 300;
        static const int CONSUMPTION = 2;
        static const int SPEED = 1;
        static const int COST = 1;
        static const int CAPACITY = 4;

    public:
        explicit Robot(Position startPosition);
        void update(const GridMap& map) override;
};