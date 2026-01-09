#include "Engine.h"
#include "Config.h"
#include "../agents/Drona.h"
#include "../agents/Robot.h"
#include "../agents/Scuter.h"
#include <fstream>

Engine :: Engine(GridMap& map, const SimulationConfig& cfg)
    : map(map), config(cfg)
{
    Config :: init(cfg);
}

void Engine :: initFleet()
{
    Position base = map.getBasePosition(); //agentii pleaca din baza si adaug toti agentii

    for (int i = 0; i < config.drones; ++i)
        ownedAgents.push_back(std :: make_unique<Drona>(base));

    for (int i = 0; i < config.robots; ++i)
        ownedAgents.push_back(std :: make_unique<Robot>(base));

    for (int i = 0; i < config.scooters; ++i)
        ownedAgents.push_back(std :: make_unique<Scuter>(base));

    agents.clear();
    for (auto& a : ownedAgents)
        agents.push_back(a.get());
}

void Engine :: spawnPackages()
{
    if (spawnedPackages >= config.totalPackages) //nu generez mai multe pachete decat limita
        return;

    if (currentTick % config.spawnFrequency != 0) //pachetele apar periodic
        return;

    const auto& clients = map.getClientsPositions(); //ma asigur ca exista clienti
    if (clients.empty())
        return;

    Position dest = clients[spawnedPackages % clients.size()];

    Package pkg;
    pkg.id = nextPackageId++;
    pkg.destination = dest;
    pkg.reward = 200 + rand() % 601; 
    pkg.deadline = currentTick + 10 + rand() % 11; 
    pkg.spawnTick = currentTick;

    packages.push_back(pkg); //adaug pachetele
    spawnedPackages++;
}

void Engine :: updateAgents() //aici dau update la starea agentului si adaug cost de operare daca se misca si nu e mort
{
    for (auto* agent : agents)
    {
        if (!agent)
            continue;

        if (!agent->isDead())
        {
            agent->update(map);
            totalOpCost += agent->getCostPerTick();
        }
    }
}

void Engine :: updatePackagesStatus()
{
    for (auto& pkg : packages)
    {
        if (pkg.delivered || pkg.failed)
            continue;

        for (auto* agent : agents)
        {
            if (!agent)
                continue;

            int reward = agent->getLastDeliveryReward();

            if (reward > 0 && !agent->hasAssignedPackage())
            {
                if (pkg.reward == reward && !pkg.delivered)
                {
                    pkg.delivered = true;
                    deliveredCount++;
                    totalReward += reward;

                    if (currentTick > pkg.deadline)
                        lateCount++;

                    break;
                }
            }
        }

        if (!pkg.delivered && currentTick > pkg.deadline + 50) //daca intarzie prea mult il consider esuat
        {
            pkg.failed = true;
            undeliveredCount++;
        }
    }

    deadAgentsCount = 0;
    for (auto* agent : agents)
        if (agent && agent->isDead())
            deadAgentsCount++;
}

void Engine :: computePenaltiesAtEnd() //adaug penalizarile
{
    totalPenalties += deadAgentsCount * config.penaltyDeadAgent;
    totalPenalties += undeliveredCount * config.penaltyUndelivered;
    totalPenalties += lateCount * config.penaltyLate;
}

void Engine :: run() //incep simularea
{
    initFleet();

    for (currentTick = 0; currentTick < config.maxTicks; ++currentTick)
    {
        spawnPackages();

        hivemind.dispatchPackages(agents, packages, map, currentTick);

        updateAgents();

        updatePackagesStatus();

        if (spawnedPackages >= config.totalPackages)
        {
            bool allDone = true;
            for (const auto& pkg : packages)
            {
                if (!pkg.delivered && !pkg.failed)
                {
                    allDone = false;
                    break;
                }
            }
            if (allDone)
                break;
        }
    }

    computePenaltiesAtEnd();
}

void Engine :: writeReport(const std :: string& filename) const
{
    std :: ofstream out(filename);
    if (!out.is_open())
        return;

    long long profit = totalReward - totalOpCost - totalPenalties;

    out << "Simulation Report\n";
    out << "=================\n\n";
    out << "Ticks run: " << currentTick << "\n";
    out << "Packages spawned: " << spawnedPackages << "\n";
    out << "Delivered: " << deliveredCount << "\n";
    out << "Late: " << lateCount << "\n";
    out << "Undelivered: " << undeliveredCount << "\n";
    out << "Dead agents: " << deadAgentsCount << "\n\n";
    out << "Total reward: " << totalReward << "\n";
    out << "Total operating cost: " << totalOpCost << "\n";
    out << "Total penalties: " << totalPenalties << "\n";
    out << "Final profit: " << profit << "\n";
}