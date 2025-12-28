#include "AgentFactory.h"
#include <stdexcept>

std :: unique_ptr<Agent> AgentFactory::createAgent(const std :: string &agentType, Position startPosition)
{
    if (agentType == "Drona")
    {
        return std :: make_unique<Drona>(startPosition);
    }
    else if (agentType == "Robot")
    {
        return std :: make_unique<Robot>(startPosition);
    }
    else if (agentType == "Scuter")
    {
        return std :: make_unique<Scuter>(startPosition);
    }
    else
    {
        throw std :: invalid_argument("Unknown agent type: " + agentType);
    }
}