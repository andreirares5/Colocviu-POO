#include "FileMapLoader.h"
#include<stdexcept>
#include<fstream>
#include <vector>

FileMapLoader :: FileMapLoader(const std :: string& filename) : filename(filename) {}

std :: unique_ptr<GridMap> FileMapLoader :: generate()
{
    std :: ifstream file(filename); //incerc sa deschid fisierul
    if(!file.is_open())
        throw std :: runtime_error("Eroare la deschiderea fisierului cu harta");

    std :: vector<std :: string> lines; //vectorul care retine liniile
    std :: string line;

    while(std :: getline(file, line)) //citesc fisierul linie cu linie
    {
        if(!line.empty())
            lines.push_back(line);
    }

    if(lines.empty()) //daca vectorul este gol, atunci nu am date valide 
        throw std :: runtime_error("Fisierul cu harta este gol");

    int rows = static_cast<int>(lines.size()); ///nr de linii citite va fi numarul de randuri
    int columns = static_cast<int>(lines[0].size()); // iar lungimea primei linii citite devine nr de coloane

    auto map = std :: make_unique<GridMap>(rows, columns); // creez in memorie

    for(int i = 0; i < rows; i++) //incep sa parcurg harta
    {
        if(static_cast<int>(lines[i].size()) != columns) //toate liniile trebuie sa aiba aceeasi lungime(=> la final obtin un dreptunghi, chiar patrat daca M = N)
            throw std :: runtime_error("Harta are linii de lungimi diferite");

        for(int j = 0; j < columns; j++)
        {
            char character = lines[i][j]; //parcurg caracterul curent sa vad ce tip de celula este
        
            switch(character)
            {
                case '.':
                    map -> setTile(i, j, TileType :: Road); //drum
                    break;

                case '#':
                    map -> setTile(i, j, TileType :: Wall); //zid
                    break;

                case 'B':
                    map -> setTile(i, j, TileType :: Base); //baza
                    break;

                case 'S':
                    map -> setTile(i, j, TileType :: Station); //statie
                    break;

                case 'D':
                    map -> setTile(i, j, TileType :: Client); //client
                    break;

                default:
                    throw std :: runtime_error("Caracter necunoscut"); //luam si
            }
        }
    }

    return map; //returnez harta complet construita
}
