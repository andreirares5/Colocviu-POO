#include "ProceduralMapGenerator.h"
#include "MapValidator.h"
#include <cstdlib>
#include <ctime>

ProceduralMapGenerator :: ProceduralMapGenerator(
    int rows, int columns, int maxStations, int clientsCount)
    : rows(rows), columns(columns),
      maxStations(maxStations), clientsCount(clientsCount)
{
    static bool value = false; //am folosit aceasta variabila pentru a ma asigura ca srand() va fi apelat o singura data
    if (!value)
    {
        std :: srand(static_cast<unsigned int>(std::time(nullptr)));
        value = true;
    }
}

std :: unique_ptr<GridMap> ProceduralMapGenerator :: generate() //de aici se va genera harta, iar unele cazuri procesul se va repeta pana cand harta este validata
{
    while (true) // din aceasta bucla ies doar cand harta este valida
    {
        auto map = std :: make_unique<GridMap>(rows, columns);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                if (std :: rand() % 100 < 20) //plasez aleator ziduri (20% pentru fiecare celula, am incercat si cu 25% si 30% si a fost prea incarcata de ziduri)
                    map->setTile(i, j, TileType :: Wall); 
            }
        }

        int bx, by; //plasez baza in mod aleator pe harta 
        do
        {
            bx = std :: rand() % rows;
            by = std :: rand() % columns;
        } while (map->getTileType(bx, by) == TileType :: Wall); //si ma asigur ca nu este plasata pe un zid

        map->setTile(bx, by, TileType :: Base);

        int placed = 0; //incep plasarea statiilor
        int attempts = 0;
        while (placed < maxStations && attempts < rows * columns * 2) //am folosit rows*columns*2 pentru a evita intrarea in bucle infinite si pentru a creste sansa de a gasi pozitii valide
        {
            int x = std :: rand() % rows;
            int y = std :: rand() % columns;

            if (map->getTileType(x, y) == TileType :: Road) //statiile vor fi plasate doar pe road
            {
                map->setTile(x, y, TileType :: Station);
                placed++;
            }
            attempts++;
        }

        placed = 0; //incep plasarea clientilor
        attempts = 0;
        while (placed < clientsCount && attempts < rows * columns * 2)
        {
            int x = std :: rand() % rows;
            int y = std :: rand() % columns;

            if (map->getTileType(x, y) == TileType :: Road)
            {
                map->setTile(x, y, TileType :: Client);
                placed++;
            }
            attempts++;
        }

        if (MapValidator :: validate(*map)) //daca totul este in regula harta este validata si generata
            return map; //returnez harta validata 
    }
}
