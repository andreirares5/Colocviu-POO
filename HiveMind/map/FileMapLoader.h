#pragma once
#include <string>
#include <memory>
#include "IMapGenerator.h"

class FileMapLoader : public IMapGenerator
{
    private:
        std :: string filename;

    public:
        explicit FileMapLoader(const std :: string& filename);
        std :: unique_ptr<GridMap> generate() override;
};