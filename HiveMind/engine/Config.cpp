#include "Config.h"

SimulationConfig& Config::get()
{
    static SimulationConfig instance;   // instanta unica
    return instance;
}

void Config::init(const SimulationConfig& cfg)
{
    get() = cfg;   // copiem configuratia citita din fisier
}
