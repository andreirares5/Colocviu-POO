#include "HiveMind.h"
#include "../agents/Drona.h"
#include "../agents/Robot.h"
#include "../agents/Scuter.h"
#include "../navigation/BFSRoute.h"
#include <algorithm>
#include <limits>
#include <cmath>

struct Opportunity //am creat aceasta structura in incercarea de a da un scor fiecarei atribuiri a unui pachet pentru un agent pentru a decide ce agent sa plece cu pachetul
{
    Agent* agent;
    int packageIndex;
    double score;
    bool viaStation; //am nevoie de statie sa livrez pachetul?
};

static Position findNearestRecharge(const GridMap& map, const Position& from) //gaseste cea mai apropiata statie/baza fata de o pozitie data
{
    Position best = from;
    int bestDist = std :: numeric_limits<int>::max();

    for (int x = 0; x < map.getColumns(); x++)
        for (int y = 0; y < map.getRows(); y++)
        {
            TileType t = map.getTileType(x, y);
            if (t == TileType :: Station || t == TileType :: Base)
            {
                int d = std :: abs(x - from.x) + std :: abs(y - from.y);
                if (d < bestDist)
                {
                    bestDist = d;
                    best = {x, y};
                }
            }
        }

    return best;
}

static double computeScore(Agent* agent, //calculez scorul pentru atribuirea pachetului unui agent
                           const Package& pkg,
                           const GridMap& map,
                           int currentTick,
                           bool viaStation)
{
    BFSRoute bfs;
    bool ignoreWalls = (dynamic_cast<Drona*>(agent) != nullptr); //drona ignora zidurile

    Position start = agent->getPosition();
    Position dest  = pkg.destination;

    int dist = 0;

    if (!viaStation) //calculez distanta fie cu escala la statie fie fara
    {
        Path p = bfs.planPath(map, start, dest, ignoreWalls); //BFS direct
        if (p.isEmpty()) //daca nu exista
            return -std :: numeric_limits<double> :: infinity(); //valoare imposibila
        dist = p.length();
    }
    else
    {
        Position station = findNearestRecharge(map, start);
        Path p1 = bfs.planPath(map, start, station, ignoreWalls); //ruta start -- statie
        Path p2 = bfs.planPath(map, station, dest, ignoreWalls); //ruta start -- destinatie
        if (p1.isEmpty() || p2.isEmpty())
             return -std :: numeric_limits<double> :: infinity();

        dist = p1.length() + p2.length(); // distanta totala (regula triunghiului)
    }

    double batteryNeeded = dist * (1.0 + 1.0 / agent->getCostPerTick()); //aici am incercat sa estimez energia necesara
    if (agent->getBatteryLevel() < batteryNeeded)
        return -std :: numeric_limits<double> :: infinity();

    double speed = std :: max<double>(1.0, (double)agent->getCostPerTick());

    double travelTime = dist / speed; //timp estimat de deplasare

    double opCost = travelTime * agent->getCostPerTick(); //cost operational

    int arrivalTick = currentTick + (int)travelTime; //tick-ul la care a sosit
    int late = std :: max<int>(0, arrivalTick - pkg.deadline); //intarzierea

    double penalty = late * (pkg.reward / (pkg.deadline + 1.0)); //penalizare

    double urgency = 1.0 / (pkg.deadline - currentTick + 1.0); //urgenta pachetului 
    double suitability = urgency / (dist + 1.0); //potrivire agent-pachet

    return pkg.reward - opCost - penalty + suitability; //scor final
}

static void assignFullPath(Agent* agent,
                           const Package& pkg,
                           const GridMap& map,
                           bool viaStation)
{
    BFSRoute bfs;
    bool ignoreWalls = (dynamic_cast<Drona*>(agent) != nullptr);

    Position start = agent->getPosition();
    Position dest  = pkg.destination;

    std :: vector<Position> full; //traseul complet

    if (!viaStation) //fara escala
    {
        Path p = bfs.planPath(map, start, dest, ignoreWalls);
        full.insert(full.end(), p.nodes.begin(), p.nodes.end());
    }
    else
    {
        Position station = findNearestRecharge(map, start);
        Path p1 = bfs.planPath(map, start, station, ignoreWalls);
        Path p2 = bfs.planPath(map, station, dest, ignoreWalls);

        full.insert(full.end(), p1.nodes.begin(), p1.nodes.end());
        full.insert(full.end(), p2.nodes.begin(), p2.nodes.end());
    }

    agent->setPath(full);
}

void HiveMind :: dispatchPackages(std :: vector<Agent*>& agents,
                                std :: vector<Package>& packages,
                                const GridMap& map,
                                int currentTick)
{
    std :: vector<Opportunity> ops; //lista oportunitatilor definite la inceput de cod

    int totalAgents = (int)agents.size(); //nr total de agenti
    int movingAgents = 0; //agenti in miscare

    for (auto* a : agents)
        if (a && a->getState() == AgentState :: MOVING) //numar agentii care se misca(vreau sa evit cresterea necontrolata a costurilor de operare)
            movingAgents++;

    int MAX_MOVING = std::max<int>(1, totalAgents / 3); //aici am vrut sa limitez numarul agentilor care se misca in acelasi timp ca sa nu creasca prea mult costul de operare
 
    if (movingAgents >= MAX_MOVING)
        return;

    for (int i = 0; i < (int)packages.size(); i++)
    {
        Package& pkg = packages[i];
        if (pkg.assigned || pkg.delivered || pkg.failed)
            continue;

        for (auto* agent : agents) //parcurg agentii
        {
            if (!agent)
                continue;
            if (agent->isDead()) 
                continue;
            if (agent->hasAssignedPackage()) 
                continue;
            if (agent->getState() != AgentState :: IDLE) 
                continue;

            double sDirect = computeScore(agent, pkg, map, currentTick, false); //calculez scorul pe ruta directa
            double sVia    = computeScore(agent, pkg, map, currentTick, true); //calculez scorul pe ruta cu escala

            double best = std :: max<double>(sDirect, sVia); //iau maximul din cele doua
            if (best <= 0.0)
                continue;

            ops.push_back({agent, i, best, sVia > sDirect}); //salvez oportunitatea
        }
    }

    if (ops.empty())
        return;

    std :: sort(ops.begin(), ops.end(), 
              [](const Opportunity& a, const Opportunity& b)
              {
                  return a.score > b.score;
              }); //sortez oportunitatile

    for (auto& op : ops)
    {
        if (movingAgents >= MAX_MOVING)
            break;

        Agent* ag = op.agent;
        Package& pkg = packages[op.packageIndex]; //extrag agentul cu pachetul asociat

        if (pkg.assigned) 
            continue;
        if (ag->hasAssignedPackage()) 
            continue;

        assignFullPath(ag, pkg, map, op.viaStation);
        ag->assignJob(pkg); //fac atribuirea
        pkg.assigned = true; // atribuirea este completa

        movingAgents++; //creste nr de agenti activi
    }
}