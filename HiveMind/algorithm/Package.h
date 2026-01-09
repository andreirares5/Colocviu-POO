#pragma once
#include "../map/GridMap.h"

struct Package
{
    int id;
    Position destination;
    int reward;
    int deadline;    
    int spawnTick;     
    bool assigned = false;
    bool delivered = false;
    bool failed = false;
};