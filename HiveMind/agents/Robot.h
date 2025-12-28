#pragma once
#include "Agent.h"

class Robot : public Agent
{
    public:
        explicit Robot(Position startPosition);
        void update(const GridMap& map) override;
};