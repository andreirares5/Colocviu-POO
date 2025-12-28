#include "MapValidator.h"
#include <queue>
#include <vector>

bool MapValidator :: validate(const GridMap& map)
{
        int rows = map.getRows();
        int columns = map.getColumns();

        std :: vector < std :: vector <bool>> visited(rows, std :: vector <bool>(columns, false));

        std :: queue<Position> queue;

        Position start = map.getBasePosition();
        queue.push(start);
        visited[start.x][start.y] = true;

        int rowOffset[4] = {-1, 1, 0, 0};
        int columnOffset[4] = {0, 0, -1, 1};

        while(!queue.empty())
        {
                Position current = queue.front();
                queue.pop();

                for(int i = 0; i < 4; i++)
                {
                        int newRow = current.x + rowOffset[i];
                        int newColumn = current.y + columnOffset[i];

                        if(map.isInside(newRow, newColumn) && !visited[newRow][newColumn] && map.isWalkable(newRow, newColumn, false))
                        {
                                visited[newRow][newColumn] = true;
                                queue.push({newRow, newColumn});
                        }
                }
        }

        for(const auto& client : map.getClientsPositions())
        {
            if(!visited[client.x][client.y])
                return false;
        }

        for(const auto& station : map.getStationsPositions())
        {
            if(!visited[station.x][station.y])
                return false;
        }

        return true;
}