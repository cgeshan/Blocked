// Pouya

#include "cost.h"

float CalculateCost(std::vector <Block> layer)
{
    float cost = 0.;
    for(auto &block : layer)
    {
        cost+=block.cost;
    }
    return cost;
}