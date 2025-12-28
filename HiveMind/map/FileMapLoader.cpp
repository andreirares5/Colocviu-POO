#include "FileMapLoader.h"
#include<stdexcept>
#include<fstream>
#include <vector>

FileMapLoader :: FileMapLoader(const std :: string& filename) : filename(filename) {}

std :: unique_ptr<GridMap> FileMapLoader :: generate()
{
    std :: ifstream file(filename);
    if(!file.is_open())
        throw std :: runtime_error("Could not open map file");

    std :: vector<std :: string> lines;
    std :: string line;

    while(std :: getline(file, line))
    {
        if(!line.empty())
            lines.push_back(line);
    }

    if(lines.empty())
        throw std :: runtime_error("Map file is empty");

    int rows = static_cast<int>(lines.size());
    int columns = static_cast<int>(lines[0].size());

    auto map = std :: make_unique<GridMap>(rows, columns);

    for(int i = 0; i < rows; i++)
    {
        if(static_cast<int>(lines[i].size()) != columns)
            throw std :: runtime_error("Map file has rows of varying lengths.");

        for(int j = 0; j < columns; j++)
        {
            char character = lines[i][j];
        
            switch(character)
            {
                case '.':
                    map -> setTile(i, j, TileType :: Road);
                    break;

                case '#':
                    map -> setTile(i, j, TileType :: Wall);
                    break;

                case 'B':
                    map -> setTile(i, j, TileType :: Base);
                    break;

                case 'S':
                    map -> setTile(i, j, TileType :: Station);
                    break;

                case 'D':
                    map -> setTile(i, j, TileType :: Client);
                    break;

                default:
                    throw std :: runtime_error("Invalid character");
            }
        }
    }

    return map;
}
