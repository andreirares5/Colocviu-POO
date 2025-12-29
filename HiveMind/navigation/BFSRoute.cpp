#include "BFSRoute.h"
#include <queue>
#include <vector>
#include <algorithm>

Path BFSRoute :: planPath(const GridMap& map, const Position& start, const Position& goal, bool ignoreWalls) const
{
    Path result;

    if(!map.isInside(start.x, start.y) || !map.isInside(goal.x, goal.y))
        return result;
        
    if(start == goal)
    {
        result.nodes.push_back(start);
        return result;
    }

    const int rows = map.getRows();
    const int columns = map.getColumns();

    std :: vector<std :: vector<bool>> visited (rows, std :: vector<bool>(columns, false));

    std :: vector<std :: vector<Position>> parent (rows, std :: vector<Position>(columns, {-1, -1}));

    std :: queue<Position> queue;

    queue.push(start);
    visited[start.x][start.y] = true;

    bool found = false;

    int rowOffset[4] = {-1, 1, 0, 0};
    int columnOffset[4] = {0, 0, -1, 1};

    while(!queue.empty() && !found)
    {
        Position current = queue.front();
        queue.pop();

        for(int i = 0; i < 4; i++)
        {
            int newRow = current.x + rowOffset[i];
            int newColumn = current.y + columnOffset[i];

            if(!map.isInside(newRow, newColumn))
                continue;

            if(visited[newRow][newColumn])
                continue;

            if(!map.isWalkable(newRow, newColumn, ignoreWalls))
                continue;

            visited[newRow][newColumn] = true;
            parent[newRow][newColumn] = current;

            Position next = {newRow, newColumn};

            if(next == goal)
            {
                found = true;
                break;
            }

            queue.push(next);
        }
    }
    
    if(!visited[goal.x][goal.y])
        return result;

    Position step = goal;
    while(!(step == start))
    {
        result.nodes.push_back(step);
        step = parent[step.x][step.y];
    }

    result.nodes.push_back(start);
    std :: reverse(result.nodes.begin(), result.nodes.end());

    return result;
}