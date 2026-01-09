#pragma once
#include <vector>
#include <cstddef>
#include "../map/GridMap.h"

struct Path
{
    std :: vector<Position> nodes; //pozitiile care formeaza traseul

    bool isEmpty() const
    {
        return nodes.empty();
    }

    size_t length() const //returnez lungimea traseului
    {
        return nodes.size();
    }
};