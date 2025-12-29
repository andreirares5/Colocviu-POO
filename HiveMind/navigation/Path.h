#pragma once
#include <vector>
#include <cstddef>
#include "../map/GridMap.h"

struct Path
{
    std :: vector<Position> nodes;

    bool isEmpty() const
    {
        return nodes.empty();
    }

    size_t length() const
    {
        return nodes.size();
    }
};