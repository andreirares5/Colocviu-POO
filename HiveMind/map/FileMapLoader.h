#pragma once
#include <string>
#include <memory>
#include "IMapGenerator.h"

class FileMapLoader : public IMapGenerator
{
    private:
        std :: string filename;

    public:
        explicit FileMapLoader(const std :: string& filename); //constructorul explicit care primeste numele fisierului(am luat sugestia de pe geeksforgeeks)
        std :: unique_ptr<GridMap> generate() override; //suprascriu metoda din IMapGenerator
};