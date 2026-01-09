#pragma once
#include <vector>
#include "../map/GridMap.h"
#include "../agents/Agent.h"
#include "Package.h"

class HiveMind
{
public:
    HiveMind() = default;

    void dispatchPackages(std::vector<Agent*>& agents,
                          std::vector<Package>& packages,
                          const GridMap& map,
                          int currentTick);
};