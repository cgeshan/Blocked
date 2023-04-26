#include <vector>

#include "Block.h"

Block::Block(int x, int y, int z, int size_x, int size_y, float cost)
{
    // set parameters
    this -> x = x;
    this -> y = y;
    this -> z = z;
    this -> size_x = size_x;
    this -> size_y = size_y;
    this -> cost = cost;

    this -> isStable = false;
    this -> id = -1;

    // set all component_elements to true. Defaults shape to rectangle
    component_elements.resize(size_x * size_y);
    for (int i = 0; i < size_x * size_y; i++)
    {
        component_elements[i] = true;
    }
}

Block::Block(int size_x, int size_y, float cost)
{
    this -> x = -1;
    this -> y = -1;
    this -> z = -1;
    this -> size_x = size_x;
    this -> size_y = size_y;
    this -> cost = cost;

    this -> isStable = false;
    this -> id = -1;

    // set all component_elements to true. Defaults shape to rectangle
    component_elements.resize(size_x * size_y);
    for (int i = 0; i < size_x * size_y; i++)
    {
        component_elements[i] = true;
    }
}

Block::Block()
{
    this -> x = -1;
    this -> y = -1;
    this -> z = -1;
    this -> size_x = -1;
    this -> size_y = -1;
    this -> id = -1;
    this -> isStable = false;
}

Block::~Block()
{
}

void Block::SetBlockElementEmpty(int i, int j)
{
    component_elements[i + j * size_x] = false;
}

bool Block::GetOccupied(int i, int j)
{
    return component_elements[i + j * size_x];
}

bool Block::IsRect()
{
    for (int x = 0; x < size_x; x++)
    {
        for (int y =0; y < size_y; y++)
        {
            if (!GetOccupied(x, y))
            {
                return false;
            }
        }
    }
    return true;
}

int Block::GetSizeX(void)
{
    return size_x;
}

int Block::GetSizeY(void)
{
    return size_y;
}