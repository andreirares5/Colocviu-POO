#pragma once
#include "Engine.h"

class Config
{
    private:
        Config() = delete; 
    public:
        // acces global la configuratie
        static SimulationConfig& get();

        // initializare o singura data din Engine
        static void init(const SimulationConfig& cfg);  
};
