#pragma once
#include <vector>
#include <memory>
#include <string>
#include "../map/GridMap.h"
#include "../agents/Agent.h"
#include "../algorithm/Package.h"
#include "../algorithm/HiveMind.h"

struct SimulationConfig
{
    int mapRows = 0;
    int mapCols = 0;
    int maxStations = 0;
    int clientsCount = 0;

    int maxTicks = 0;
    int totalPackages = 0;
    int spawnFrequency = 0;

    int drones = 0;
    int robots = 0;
    int scooters = 0;

    int penaltyDeadAgent = 500;
    int penaltyUndelivered = 200;
    int penaltyLate = 50;
};

class Engine
{
private:
    GridMap& map;
    SimulationConfig config;
    HiveMind hivemind;

    std::vector<std::unique_ptr<Agent>> ownedAgents;
    std::vector<Agent*> agents;
    std::vector<Package> packages;

    int currentTick = 0;
    int nextPackageId = 0;
    int spawnedPackages = 0;

    int deliveredCount = 0;
    int lateCount = 0;
    int undeliveredCount = 0;
    int deadAgentsCount = 0;

    long long totalReward = 0;
    long long totalOpCost = 0;
    long long totalPenalties = 0;

public:
    Engine(GridMap& map, const SimulationConfig& cfg);

    void initFleet();
    void run();
    void writeReport(const std::string& filename) const;

private:
    void spawnPackages();
    void updateAgents();
    void updatePackagesStatus();
    void computePenaltiesAtEnd();
};