#pragma once
#include"Agent.h"

class Drona : public Agent
{
    private:
        static const int BATTERY = 100;
        static const int CONSUMPTION = 10;
        static const int SPEED = 3;
        static const int COST = 15;
        static const int CAPACITY = 1;
        
    public:
        explicit Drona(Position startPosition);
        void update(const GridMap& map) override;
};