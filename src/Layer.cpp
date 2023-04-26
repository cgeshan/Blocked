// Abraham

#include <vector>
#include <iostream>
#include <unordered_set>

#include "Layer.h"
#include "Block.h"
#include "cost.h"

// Layer class, to hold all of the information needed for one layer of blocks.
Layer::Layer(int size_x, int size_y, int z, std::vector<Block> block_vector)
{
    // set size and z hight.
    this->size_x = size_x;
    this->size_y = size_y;
    this->z = z;

    // populate the block map.
    block_map.resize(size_x * size_y);
    for (int i = 0; i < size_x * size_y; ++i)
    {
        block_map[i] = -1;
    }

    // For each block, set the block ID and set the block map.
    for (Block &block : block_vector)
    {
        block.id = block_counter;
        this->block_list[block_counter] = block;
        block.z = z;
        // loop through all of the elements of the block, setting all of the voxels 
        // it occupies accordingly.
        for (int i = 0; i < block.GetSizeX(); ++i)
        {
            for (int j = 0; j < block.GetSizeY(); ++j)
            {
                if (block.GetOccupied(i, j))
                {
                    SetVoxel(block.x + i, block.y + j, block_counter);
                }
            }
        }
        block_counter++; // counter to make sure that each block has a unique ID.
    }
}

Layer::Layer()
{

}

void Layer::SetVoxel(int x, int y, int block_ID)
{
    // If the voxel exists in the layer, set its entry in the block map to the block ID 
    if (x < 0 || x >= size_x || y < 0 || y >= size_y)
    {
        return;
    }
    block_map[x + size_x * y] = block_ID;
    correct_cost = false; // the layer has been edited. The stored cost can no longer be trusted!
}

bool Layer::GetOccupancy(int x, int y)
// Check if a block exists at the location x, y
{
    if (x < 0 || x >= size_x || y < 0 || y >= size_y)
    {
        return false;
    }
    if (block_map[x + size_x * y] == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Block *Layer::GetBlockPointer(int x, int y)
// return a pointer to the block at (x, y)
{
    int nblock = GetBlockID(x, y);
    return GetBlockPointer(nblock);
}

// return a pointer to the block with the specified block ID
Block *Layer::GetBlockPointer(int blockID)
{
    auto found = block_list.find(blockID);
    if (found == block_list.end())
    {
        return nullptr;
    }
    return &(found->second);
}

// Get the block ID of the block at location (x, y)
int Layer::GetBlockID(int x, int y)
{
    if (x < 0 || x >= size_x || y < 0 || y >= size_y)
    {
        return -1;
    }
    return block_map[x + size_x * y];
}

// delete the block the pointer points to.
void Layer::DeleteBlock(Block *block)
{
    if (block == nullptr)
    {
        return;
    }
    int erase_id = block->id;
    // Make sure to remove the block's ID from the map.
    for (int i = 0; i < block->GetSizeX(); ++i)
    {
        for (int j = 0; j < block->GetSizeY(); ++j)
        {
            // For each entry the the block occupied, if the blockID is the block to be erased, set the ID to -1.
            if (block->GetOccupied(i, j) && erase_id == GetBlockID(i + block->x, j + block->y))
            {
                SetVoxel(i + block->x, j + block->y, -1);
            }
        }
    }
    block_list.erase(erase_id);
    correct_cost = false; // the layer has been edited. The stored cost can no longer be trusted!
}

// delete a block based on ID.
void Layer::DeleteBlock(int blockID)
{
    Block *erase_block = GetBlockPointer(blockID);
    if (GetBlockPointer(blockID) == nullptr)
    {
        return;
    }
    DeleteBlock(erase_block);
}

void Layer::DeleteBlock(int x, int y)
{
    DeleteBlock(GetBlockID(x, y));
}

// Adds a new block to the block_list. This is DANGEROUS, as this block can cause issues
// if not also added to the block map. Returns the new block's ID.
int Layer::AddBlock(Block &block)
{
    block.id = block_counter;
    this->block_list[block_counter] = block;
    block_counter++;
    correct_cost = false; // the layer has been edited. The stored cost can no longer be trusted!
    return block_counter - 1;
}

std::vector<Block> Layer::GetBlockVector(void)
// returns a COPY of the blocks in the unordered map.
{
    std::vector<Block> block_vector;
    for (auto kv : block_list)
    {
        block_vector.push_back(kv.second);
    }
    return block_vector;
    // VerifyOneToOne() // Can be run to make sure the block vector is being generated right.
}

// Get the cost of the layer (from the cost function). If the layer has not been altered, then
// the last calculated cost function is returned.
float Layer::GetCost(void)
{
    if (!correct_cost)
    {
        cost = CalculateCost(GetBlockVector());
        correct_cost = true;
    }

    // can be run to double check that correct_cost is working right.
    // else 
    // {
    //     if (CalculateCost(GetBlockVector()) != cost)
    //     {
    //         std::cout << "Get cost messed up!" << std::endl;
    //     }
    // }
    return cost;
}

// verify that every block in the block_map apears in the block list and every block
// in the block list appears in the block_map.
void Layer::VerifyOneToOne(void)
{
    std::unordered_set<int> existing_blocks;
    for (int x = 0; x < size_x; ++x)
    {
        for (int y = 0; y < size_y; ++y)
        {
            existing_blocks.insert(GetBlockID(x, y));
        }
    }
    for (auto &kv : block_list)
    {
        if (existing_blocks.count(kv.first) != 1)
        {
            std::cout << "Layer has failed one to one check. A block is missing!" << std::endl;
        }
    }
    if (existing_blocks.size() < block_list.size())
    {
        std::cout << "Layer has failed one to one check. There's an extra block!" << std::endl;
    }
    if (existing_blocks.size() > block_list.size())
    {
        std::cout << "Layer has failed one to one check, existing blocks more then block list." << std::endl;
    } 
}

int Layer::CountUnstableBlocks(void)
{
    int count = 0;
    for (auto &kv : block_list)
    {
        if (!kv.second.isStable)
        {
            ++count;
        }
    }
    return count;
}

void Layer::SetLayerStable(void)
// Sets all of the blocks in the layer to isStable = true.
// Used to set the bottom layer to stable.
{
    for (auto &kv : block_list)
    {
        kv.second.isStable = true;
    }
}