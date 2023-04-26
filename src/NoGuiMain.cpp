#include <iostream>
#include <vector>

#include "Merge.h"
#include "Block.h"
#include "Voxelize.h"
#include "HelperFunctions.h"

// Works well with cone.stl.
int main(int arc, char* argv[])
// Main function for use without GUI.
{
    std::vector<Block> possibleBlocks;
    // possibleBlocks.push_back(Block(0, 0, 0, 2, 2));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 2, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 2, 1.0));
    // possibleBlocks.push_back(Block(0, 0, 0, 3, 1, 1.0));
    // possibleBlocks.push_back(Block(0, 0, 0, 1, 3, 1.0));
    // possibleBlocks.push_back(Block(0, 0, 0, 2, 3, 1.0));
    // possibleBlocks.push_back(Block(0, 0, 0, 3, 2, 1.0));
    // possibleBlocks.push_back(Block(0, 0, 0, 3, 3, 1.0));

    // Make L blocks
    // for (int i = 0; i < 2; ++i)
    // {
    //     for (int j=0; j < 2; ++j)
    //     {
    //         Block l_block(0, 0, 0, 2, 2, 1.0);
    //         l_block.SetBlockElementEmpty(i, j);
    //         possibleBlocks.push_back(l_block);
    //     }
    // }
    std::vector<std::vector<Block> > layers;
    if (arc > 1)
    {
        std::cout << "start voxilization" << std::endl;
        layers = STLToBlocksInt(argv[1], 40);
        // layers = STLToBlocks(argv[1], 1.0);
        std::cout << "end voxilization" << std::endl;
    }
    else 
    {
        layers = create_test_structure();
    }
    // printBlocks(layers, false);
    MergeThreaded(layers, possibleBlocks, false);
    std::cout<<"-------------------------\n\n";
    // printBlocks(layers, true);
}