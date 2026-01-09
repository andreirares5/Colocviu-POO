#include "AgentFactory.h"
#include "Drona.h"
#include "Robot.h"
#include "Scuter.h"
#include <stdexcept>

std::unique_ptr<Agent> AgentFactory :: createAgent(
    const std :: string& agentType,
    Position startPosition)
{
    if (agentType == "Drona")
        return std :: make_unique<Drona>(startPosition);

    if (agentType == "Robot")
        return std :: make_unique<Robot>(startPosition);

    if (agentType == "Scuter")
        return std :: make_unique<Scuter>(startPosition);

    throw std :: invalid_argument("Tip necunoscut " + agentType);
}
