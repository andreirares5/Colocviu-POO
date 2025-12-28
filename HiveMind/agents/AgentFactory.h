#pragma once
#include <memory>
#include <string>

#include "Drona.h"
#include "Robot.h"
#include "Scuter.h"
#include "Agent.h"

class AgentFactory
{
    public:
        static std :: unique_ptr<Agent> createAgent(const std :: string &agentType, Position startPosition);
};