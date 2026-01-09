#include <iostream>
#include <fstream>
#include <string>

#include "map/ProceduralMapGenerator.h"
#include "engine/Engine.h"

SimulationConfig loadConfig(const std :: string& filename)
{
    SimulationConfig cfg;

    std :: ifstream in(filename);
    if (!in.is_open())
        throw std :: runtime_error("Nu am putut deschide simulation_setup.txt");

    std :: string key;
    while (in >> key)
    {
        if (key == "MAP_SIZE:")
            in >> cfg.mapRows >> cfg.mapCols;

        else if (key == "MAX_TICKS:")
            in >> cfg.maxTicks;

        else if (key == "MAX_STATIONS:")
            in >> cfg.maxStations;

        else if (key == "CLIENTS_COUNT:")
            in >> cfg.clientsCount;

        else if (key == "DRONES:")
            in >> cfg.drones;

        else if (key == "ROBOTS:")
            in >> cfg.robots;

        else if (key == "SCOOTERS:")
            in >> cfg.scooters;

        else if (key == "TOTAL_PACKAGES:")
            in >> cfg.totalPackages;

        else if (key == "SPAWN_FREQUENCY:")
            in >> cfg.spawnFrequency;

        else
        {
            std :: string rest;
            std :: getline(in, rest);
        }
    }

    return cfg;
}

int main()
{
    try
    {
        SimulationConfig cfg = loadConfig("simulation_setup.txt");

        ProceduralMapGenerator generator(
            cfg.mapRows,
            cfg.mapCols,
            cfg.maxStations,
            cfg.clientsCount
        );

        std :: unique_ptr<GridMap> map = generator.generate();


        map->printMap("generated_map.txt");
        std :: cout << "Harta generata a fost salvata in generated_map.txt\n";

        Engine engine(*map, cfg);
        engine.run();

        engine.writeReport("simulation.txt");
        std :: cout << "Simularea s-a incheiat. Raportul este in simulation.txt\n";
    }
    catch (const std :: exception& ex)
    {
        std :: cerr << "Eroare: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}