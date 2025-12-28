#pragma once
#include"Agent.h"

class Drona : public Agent
{
    public:
        explicit Drona(Position startPosition);
        void update(const GridMap& map) override;
};