#include "MapValidator.h"
#include <queue>
#include <vector>

bool MapValidator :: validate(const GridMap& map)
{
        int rows = map.getRows();
        int columns = map.getColumns();

        std :: vector < std :: vector <bool>> visited(rows, std :: vector <bool>(columns, false)); //stochez pozitiile vizitate din BFS

        std :: queue<Position> queue; //coada folosita pt BFS

        Position start = map.getBasePosition(); //punctul de start va fi baza
        queue.push(start);
        visited[start.x][start.y] = true;

        int rowOffset[4] = {-1, 1, 0, 0}; //deplasarea in cele 4 directii
        int columnOffset[4] = {0, 0, -1, 1};

        while(!queue.empty())
        {
                Position current = queue.front();
                queue.pop();

                for(int i = 0; i < 4; i++) //explorarea vecinilor pozitiei curente
                {
                        int newRow = current.x + rowOffset[i];
                        int newColumn = current.y + columnOffset[i];

                        if(map.isInside(newRow, newColumn) && !visited[newRow][newColumn] && map.isWalkable(newRow, newColumn, false))
                        {
                                visited[newRow][newColumn] = true;
                                queue.push({newRow, newColumn});
                        } //verific daca pozitia nu a fost vizitata, daca este in harta si daca este accesibila
                }
        }

        for(const auto& client : map.getClientsPositions()) //verific toti clientii sa fie accesibili din baza
        {
            if(!visited[client.x][client.y])
                return false;
        }

        for(const auto& station : map.getStationsPositions()) //verific statiile sa fie accesibile din baza
        {
            if(!visited[station.x][station.y])
                return false;
        }

        return true;
}