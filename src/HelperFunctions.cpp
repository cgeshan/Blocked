#include <vector>
#include <iostream>

#include "Block.h"
#include "HelperFunctions.h"
#include "Layer.h"

std::vector<std::vector<Block> > create_test_structure(void)
{
    // Create a test structure that is a 3 layers high. The first two layers are 2x2, 
    // and the top layer is 4x2, making a 2 block wide T shape.
    std::vector<std::vector<Block> > layers;
    // std::cout<<__LINE__<<std::endl;
    layers.resize(3);
    // std::cout<<__LINE__<<std::endl;
    //bottom 2 layers
    for (int z = 0; z < 2; ++z)
    {
        for(int x = 1; x < 3; ++x)
        {
            for (int y = 0; y < 2; ++y)
            {
                layers[z].push_back(Block(x, y, z, 1, 1, 1.0));
                // std::cout<<__LINE__<<" "<<z<<" "<<x<<" "<<y<<std::endl;
            }
        }
    }
    //top layer
    // std::cout<<__LINE__<<std::endl;
    for(int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 2; ++y)
        {
            layers[2].push_back(Block(x, y, 3, 1, 1, 1.0));
            // std::cout<<__LINE__<<" "<<3<<" "<<x<<" "<<y<<std::endl;
        }
    }
    return layers;
}

void printBlocks(std::vector<std::vector<Block> > layers, bool print_stability)
{
    int max_x = 0;
    int max_y = 0;
    int max_z = layers.size();
    for (std::vector<Block> &layer : layers)
    {
        for (Block &block : layer)
        {
            if (max_x < block.x + block.GetSizeX())
            {
                max_x = block.x + block.GetSizeX();
            }
            if (max_y < block.y + block.GetSizeY())
            {
                max_y = block.y + block.GetSizeY();
            }
        }
    }
    int block_number = 1;
    for (std::vector<Block> &layer : layers)
    {
        std::vector<std::vector<int> > layer_map;
        layer_map.resize(max_x);
        for (int i = 0; i < max_x; ++i)
        {
            layer_map[i].resize(max_y);
            for (int j =0; j < max_y; ++j)
            {
                layer_map[i][j] = 0;
            }
        }
        for (Block &block : layer)
        {
            for (int i = 0; i < block.GetSizeX(); ++i)
            {
                for (int j = 0; j < block.GetSizeY(); ++j)
                {
                    if (block.GetOccupied(i, j))
                    {
                        if (!print_stability || block.isStable)
                        {
                            layer_map[block.x + i][block.y + j] = block_number;
                        }
                        else
                        {
                            layer_map[block.x + i][block.y + j] = -1;
                        }
                    }
                }
            }
            ++block_number;
        }
        for (int j = 0; j < max_y; ++j)
        {
            for(int i=0; i < max_x; ++i)
            {
                std::cout << layer_map[i][j] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
}

void printStabilities(std::vector<std::vector<Block> > layers)
{
    int max_x = 0;
    int max_y = 0;
    int max_z = layers.size();
    for (std::vector<Block> &layer : layers)
    {
        for (Block &block : layer)
        {
            if (max_x < block.x + block.GetSizeX())
            {
                max_x = block.x + block.GetSizeX();
            }
            if (max_y < block.y + block.GetSizeY())
            {
                max_y = block.y + block.GetSizeY();
            }
        }
    }
    int block_number = 1;
    for (std::vector<Block> &layer : layers)
    {
        std::vector<std::vector<int> > layer_map;
        layer_map.resize(max_x);
        for (int i = 0; i < max_x; ++i)
        {
            layer_map[i].resize(max_y);
            for (int j =0; j < max_y; ++j)
            {
                layer_map[i][j] = 0;
            }
        }
        for (Block &block : layer)
        {
            for (int i = 0; i < block.GetSizeX(); ++i)
            {
                for (int j = 0; j < block.GetSizeY(); ++j)
                {
                    if (block.GetOccupied(i, j))
                    {
                        if (block.isStable)
                        {
                            layer_map[block.x + i][block.y + j] = 1;
                        }
                        else
                        {
                            layer_map[block.x + i][block.y + j] = -1;
                        }
                    }
                }
            }
            ++block_number;
        }
        for (int j = 0; j < max_y; ++j)
        {
            for(int i=0; i < max_x; ++i)
            {
                std::cout << layer_map[i][j] << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
}

//Helper function for visualization. Prints a layer to the terminal.
void PrintLayer(Layer layer)
{
    std::cout << "\n";
    for (int x = 0; x < layer.size_x; ++x)
    {
        for (int y =0; y < layer.size_y; ++y)
        {
            if (layer.GetOccupancy(x, y) && !layer.GetBlockPointer(x, y)->irreplaceable)
            {
                std::cout << 0 << "\t";
            }
            else
            {
                std::cout << layer.GetBlockID(x, y) << "\t";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void PrintLayerStability(Layer layer)
{
    std::cout << "\n";
    for (int x = 0; x < layer.size_x; ++x)
    {
        for (int y =0; y < layer.size_y; ++y)
        {
            if (layer.GetOccupancy(x, y))
            {
                if (layer.GetBlockPointer(x,y)->isStable)
                {
                    std::cout << 1 << "\t";
                }
                else
                {
                    std::cout << -1 << "\t";
                }
                
            }
            else
            {
                std::cout << 0 << "\t";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n"; 
}