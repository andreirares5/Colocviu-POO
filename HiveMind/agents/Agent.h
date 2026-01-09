#pragma once
#include "AgentState.h"
#include "../map/GridMap.h"
#include "../algorithm/Package.h"
#include <vector>

class Agent
{
protected:
    Position position;
    Position destination;
    AgentState state;
    std::vector<Position> path;

    int batteryLevel;
    int maxBatteryLevel;

    int speed;
    int consumption;
    int costPerTick;
    int capacity;

    bool hasPackage;
    Package currentPackage;
    int lastDeliveryReward;

public:
    Agent(Position startPosition,
          int maxBat,
          int consumption,
          int speed,
          int cost,
          int capacity);

    virtual ~Agent() = default;

    virtual void update(const GridMap& map); //actualizez la fiecare tick

    void assignJob(const Package& pkg); //ii dau un job agentului
    void completeDelivery(); //marchez finalizarea comenzii

    AgentState getState() const;
    Position getPosition() const;
    int getBatteryLevel() const;
    int getCostPerTick() const;
    int getLastDeliveryReward() const;
    bool hasAssignedPackage() const;
    bool isDead() const;
    void setPath(const std::vector<Position>& newPath); //varianta alternativa pentru o ruta noua

protected: 
    void consumeBattery(); //le-am pus protected sa poata fi accesate de clasele care mostenesc Agent si sa nu fie folosite in tot programul
    void recharge();
    Position findNearestStation(const GridMap& map, const Position& from); //cauta cea mai apropiata statie de o pozitie
    void recomputePathTo(const GridMap& map, const Position& target, bool ignoreWalls); //recalculez traseul catre un punct tinta
};