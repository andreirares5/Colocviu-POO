#include "Agent.h"
#include "../navigation/BFSRoute.h"
#include "../map/Tile.h"
#include <cmath>

Agent :: Agent(Position startPosition,
             int maxBat,
             int consumption,
             int speed,
             int cost,
             int capacity)
    : position(startPosition),
      destination(startPosition),
      state(AgentState::IDLE),
      batteryLevel(maxBat),
      maxBatteryLevel(maxBat),
      speed(speed),
      consumption(consumption),
      costPerTick(cost),
      capacity(capacity),
      hasPackage(false),
      lastDeliveryReward(0)
{}

AgentState Agent :: getState() const  //getters
{ 
    return state; 
}
Position Agent :: getPosition() const 
{ 
    return position; 
}
int Agent :: getBatteryLevel() const 
{ 
    return batteryLevel; 
}
int Agent :: getCostPerTick() const 
{ 
    return costPerTick; 
}
bool Agent :: hasAssignedPackage() const 
{ 
    return hasPackage; 
}
bool Agent :: isDead() const 
{ 
    return state == AgentState :: DEAD; 
}
int Agent :: getLastDeliveryReward() const 
{ 
    return lastDeliveryReward; 
}


void Agent :: assignJob(const Package& pkg) //ii dau unui agent un pachet si incep deplasarea
{
    currentPackage = pkg;
    hasPackage = true;
    destination = pkg.destination;
    state = AgentState :: MOVING;
    lastDeliveryReward = 0;
}

void Agent :: completeDelivery() //duc la capat livrarea, agentul primeste recompensa si intra in starea IDLE
{
    if (hasPackage)
    {
        lastDeliveryReward = currentPackage.reward;
        hasPackage = false;
        state = AgentState :: IDLE;
    }
}

void Agent :: consumeBattery() //consumul bateriei in functie de tipul agentului 
{
    batteryLevel -= consumption;
    if (batteryLevel <= 0)
    {
        batteryLevel = 0;
        state = AgentState :: DEAD;
    }
}

void Agent :: recharge() //incarcare
{
    batteryLevel += maxBatteryLevel / 4;
    if (batteryLevel > maxBatteryLevel)
        batteryLevel = maxBatteryLevel;
}


void Agent::setPath(const std :: vector<Position>& newPath)
{
    path = newPath;
}


Position Agent :: findNearestStation(const GridMap& map, const Position& from) //caut cea mai apropiata statie
{
    Position best = from;
    int bestDist = 999999;

    for (int x = 0; x < map.getColumns(); ++x) //parcurg toata harta
    {
        for (int y = 0; y < map.getRows(); ++y)
        {
            TileType t = map.getTileType(x, y); //statiile si baza vor fi puncte valide de incarcare
            if (t == TileType :: Station || t == TileType :: Base)
            {
                int d = std :: abs(x - from.x) + std :: abs(y - from.y); //caut distanta cea mai eficienta
                if (d < bestDist)
                {
                    bestDist = d;
                    best = { x, y };
                }
            }
        }
    }

    return best;
}

void Agent :: recomputePathTo(const GridMap& map, const Position& target, bool ignoreWalls)
{
    BFSRoute bfs;
    Path p = bfs.planPath(map, position, target, ignoreWalls);
    path = p.nodes;
    destination = target;
}


void Agent :: update(const GridMap& map) //update la fiecare tick
{
    if (state == AgentState :: DEAD) //daca agentul e mort nu se mai poate face nimic cu el
        return;

    TileType tile = map.getTileType(position.x, position.y);

    if (tile == TileType :: Station || tile == TileType :: Base) //in statie sau baza nu consum nimic si nu creste costul de operare
    {
        recharge();
        state = AgentState :: CHARGING;
        return;
    }

    bool ignoreWalls = false;

    if (hasPackage)
    {
        int stepsRemaining = static_cast<int>(path.size()); //calculez energia necesara pentru a parcurge traseul curent
        int energyNeeded = stepsRemaining * consumption;

        if (batteryLevel < energyNeeded + maxBatteryLevel / 5) //daca nu am suficienta baterie duc agentul la statie sa se reincarce
        {
            Position station = findNearestStation(map, position);
            recomputePathTo(map, station, ignoreWalls);
            state = AgentState :: MOVING;
        }
    }
    else
    {
        if (batteryLevel < maxBatteryLevel / 2) //daca nu am pachet si baterie sub jumatate, il incarc 
        {
            Position station = findNearestStation(map, position);
            recomputePathTo(map, station, ignoreWalls);
            state = AgentState :: MOVING;
        }
    }

    if (state == AgentState :: MOVING)
    {
        int steps = 0;

        while (steps < speed && !path.empty() && state != AgentState :: DEAD) //aici abordez agentii care se deplaseaza cu mai multe celule/tick
        {
            consumeBattery();
            if (state == AgentState :: DEAD)
                return;

            position = path.front();
            path.erase(path.begin());
            steps++;
        }

        if (position.x == destination.x && position.y == destination.y) //daca a ajuns la destinatie 
        {
            if (hasPackage)
            {
                completeDelivery(); //a livrat pachetul cu succes
                Position station = findNearestStation(map, position);
                recomputePathTo(map, station, ignoreWalls);
                state = AgentState::MOVING;
                return;
            }

            state = AgentState :: IDLE; //daca nu are pachet este IDLE
        }
    }
}