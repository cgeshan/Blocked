#include <iostream>
#include <vector>
#include <cstring>

#include "../Merge.h"
#include "../Block.h"
#include "../Voxelize.h"
#include "../HelperFunctions.h"

bool testResult(std::vector<std::vector<Block> > layers)
// Tests the result from merge for create_test_structure() using only 1x1, 1x2, and 2x1 blocks.
{
    //The optimal solution has exactly 8 blocks (4+4+8voxels, with the cheapes option being all 1x2 blocks)
    // Additionally, inorder to make the top stable, it needs to be make out of all 2x1 blocks, of size:
    //size_x = 2, size_y =1 (they need to be pointed horizontally)
    int total_blocks = 0;
    for (auto layer : layers)
    {
        for (auto block : layer)
        {
            ++total_blocks;
        }
    }
    bool top_layer_horizontal = true;
    for (auto block : layers[2])
    {
        if (block.GetSizeX() != 2 || block.GetSizeY() != 1)
        {
            top_layer_horizontal = false;
            std::cout << block.GetSizeX() << block.GetSizeY() << std::endl;
        }
    }
    if (total_blocks == 8 && top_layer_horizontal)
    {
        return true;
    }
    else
    {
        std::cout << total_blocks << " " << top_layer_horizontal;
        return false;
    }
}

// Test 1 tests to see if the test shape can be successfuly built by merge if stabilize is set to true.
bool RunTest1(void)
{
    std::vector<Block> possibleBlocks;
    possibleBlocks.push_back(Block(0, 0, 0, 1, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 1, 10.0));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 2, 1.0));
    std::vector<std::vector<Block> > layers = create_test_structure();
    Merge(layers, possibleBlocks, true);

    //The optimal solution has exactly 8 blocks (4+4+8voxels, with the cheapes option being all 1x2 blocks)
    // Additionally, inorder to make the top stable, it needs to be make out of all 2x1 blocks, of size:
    //size_x = 2, size_y =1 (they need to be pointed horizontally).
    int total_blocks = 0;
    for (auto layer : layers)
    {
        for (auto block : layer)
        {
            ++total_blocks;
        }
    }
    bool top_layer_horizontal = true;
    for (auto block : layers[2])
    {
        if (block.GetSizeX() != 2 || block.GetSizeY() != 1)
        {
            top_layer_horizontal = false;
        }
    }
    if (total_blocks == 8 && top_layer_horizontal)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Test 2 tests to see if the test shape can be successfuly built by merge if stabilize is set to false.
bool RunTest2(void)
{
    std::vector<Block> possibleBlocks;
    possibleBlocks.push_back(Block(0, 0, 0, 1, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 1, 10.0));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 2, 1.0));
    std::vector<std::vector<Block> > layers = create_test_structure();
    Merge(layers, possibleBlocks, false);

    //The optimal solution has exactly 8 blocks (4+4+8voxels, with the cheapes option being all 1x2 blocks)
    // Additionally, inorder to make the top min cost, it needs to be make out of all 2x1 blocks, of size:
    // size_x = 1, size_y = 2 (they need to be pointed vertically)
    int total_blocks = 0;
    for (auto layer : layers)
    {
        for (auto block : layer)
        {
            ++total_blocks;
        }
    }
    bool top_layer_horizontal = true;
    for (auto block : layers[2])
    {
        if (block.GetSizeX() != 1 || block.GetSizeY() != 2)
        {
            top_layer_horizontal = false;
        }
    }
    if (total_blocks == 8 && top_layer_horizontal)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Test 3 tests threading. Same as test 1, but using MergeThreaded.
bool RunTest3(void)
{
    std::vector<Block> possibleBlocks;
    possibleBlocks.push_back(Block(0, 0, 0, 1, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 1, 10.0));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 2, 1.0));
    std::vector<std::vector<Block> > layers = create_test_structure();
    MergeThreaded(layers, possibleBlocks, true);

    //The optimal solution has exactly 8 blocks (4+4+8voxels, with the cheapes option being all 1x2 blocks)
    // Additionally, inorder to make the top stable, it needs to be make out of all 2x1 blocks, of size:
    //size_x = 2, size_y =1 (they need to be pointed horizontally).
    int total_blocks = 0;
    for (auto layer : layers)
    {
        for (auto block : layer)
        {
            ++total_blocks;
        }
    }
    bool top_layer_horizontal = true;
    for (auto block : layers[2])
    {
        if (block.GetSizeX() != 2 || block.GetSizeY() != 1)
        {
            top_layer_horizontal = false;
        }
    }
    if (total_blocks == 8 && top_layer_horizontal)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Test 4 tests threading. Same as test 2, but using MergeThreaded.
bool RunTest4(void)
{
    std::vector<Block> possibleBlocks;
    possibleBlocks.push_back(Block(0, 0, 0, 1, 1, 1.0));
    possibleBlocks.push_back(Block(0, 0, 0, 2, 1, 10.0));
    possibleBlocks.push_back(Block(0, 0, 0, 1, 2, 1.0));
    std::vector<std::vector<Block> > layers = create_test_structure();
    MergeThreaded(layers, possibleBlocks, false);

    //The optimal solution has exactly 8 blocks (4+4+8voxels, with the cheapes option being all 1x2 blocks)
    // Additionally, inorder to make the top min cost, it needs to be make out of all 2x1 blocks, of size:
    // size_x = 1, size_y = 2 (they need to be pointed vertically)
    int total_blocks = 0;
    for (auto layer : layers)
    {
        for (auto block : layer)
        {
            ++total_blocks;
        }
    }
    bool top_layer_horizontal = true;
    for (auto block : layers[2])
    {
        if (block.GetSizeX() != 1 || block.GetSizeY() != 2)
        {
            top_layer_horizontal = false;
        }
    }
    if (total_blocks == 8 && top_layer_horizontal)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Verfiies that merge returns the right results for create_test_structure() using only 1x1, 1x2, and 2x1 blocks.
int main(int arc, char* argv[])
{
    bool test1 = false;
    bool test2 = false;
    bool test3 = false;

    if (arc > 1)
    {
        for (int i = 1; i < arc; ++i)
        {
            if (std::strcmp(argv[i], "Test1") != 0)
            {
                test1 = true;
            }
            if (std::strcmp(argv[i], "Test2") != 0)
            {
                test2 = true;
            }
            if (std::strcmp(argv[i], "Test3") != 0)
            {
                test3 = true;
            }
        }
    }
    else
    {
        std::cout << "No valid test selected. Using all tests." << std::endl;
        test1 = true;
        test2 = true;
        test3 = true;
    }
    if (!test1 && !test2 && !test3)
    {
        std::cout << "No valid test selected. Using all tests." << std::endl;
        test1 = true;
        test2 = true;
        test3 = true;
    }

    if (test1)
    {
        if (!RunTest1())
        {
            std::cout << "Failed Test 1" << std::endl;
            return 1;
        }
    }
    std::cout << "Passed 1!" << std::endl;
    if (test2)
    {
        if (!RunTest2())
        {
            std::cout << "Failed Test 2" << std::endl;
            return 1;
        }
    }
    std::cout << "Passed 2!" << std::endl;
    if (test3)
    {
        if (!RunTest3())
        {
            std::cout << "Failed Test 3" << std::endl;
            return 1;
        }
    }
    std::cout << "Passed!" << std::endl;
    return 0;
}