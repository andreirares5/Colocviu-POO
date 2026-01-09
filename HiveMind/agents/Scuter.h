#pragma once
#include "Agent.h"

class Scuter : public Agent
{
    private:
        static const int BATTERY = 200;
        static const int CONSUMPTION = 5;
        static const int SPEED = 2;
        static const int COST = 4;
        static const int CAPACITY = 2;

    public:
        explicit Scuter(Position startPosition);
        void update(const GridMap& map) override;
};