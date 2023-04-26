// Abraham
#ifndef LAYER_IS_INCLUDED
#define LAYER_IS_INCLUDED

#include <vector>
#include <unordered_map>

#include "Block.h"

// LayerArray header file
class Layer
{
public:
    int block_counter = 0;
    int size_x = 0;
    int size_y = 0;
    int z = -1;
    int start_index = 0;
    int itercount = 0;
    std::unordered_map <int, Block> block_list;
    Layer(int size_x, int size_y, int z, std::vector<Block>);
    Layer();
    void SetVoxel(int x, int y, int blockID);
    bool GetOccupancy(int x, int y);
    Block* GetBlockPointer(int x, int y);
    Block* GetBlockPointer(int blockID);
    int GetBlockID(int x, int y);
    void DeleteBlock(Block* block_ptr);
    void DeleteBlock(int blockID);
    void DeleteBlock(int x, int y);
    int AddBlock(Block &new_block);
    int CountUnstableBlocks(void);
    std::vector<Block> GetBlockVector(void);
    float GetCost(void);
    void SetLayerStable(void);

    //Testing Funcitons
    void VerifyOneToOne(void);

private:
    std::vector<int> block_map;
    void ComputeBlockMap(void);
    float cost = 0;
    bool correct_cost = false;
};


#endif