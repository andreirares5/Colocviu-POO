#pragma once
#include <memory>
#include <string>
#include "Agent.h"

class AgentFactory
{
public:
    static std :: unique_ptr<Agent> createAgent(
        const std :: string& agentType,
        Position startPosition
    );
};
