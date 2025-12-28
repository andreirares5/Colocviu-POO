#pragma once
#include "Agent.h"

class Scuter : public Agent
{
    public:
        explicit Scuter(Position startPosition);
        void update(const GridMap& map) override;
};