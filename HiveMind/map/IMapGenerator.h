#pragma once
#include<memory>
#include<GridMap.h>

class IMapGenerator
{
    public:
        virtual ~IMapGenerator() = default;
        virtual std :: unique_ptr<GridMap> generate() = 0;
};