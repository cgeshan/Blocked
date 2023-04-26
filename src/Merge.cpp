// Abraham

#include <vector>
#include <iostream>
#include <deque>
#include <algorithm>

#include "wx/wx.h"

#include "App.h"
#include "MainWindow.h"
#include "Merge.h"
#include "Block.h"
#include "Stability.h"
#include "cost.h"
#include "Layer.h"
#include "HelperFunctions.h"
#include "WorkerThread.h"

DECLARE_APP(App);
std::mutex mtx;

void test(std::vector<Block> BlockOptions)
{
    for(int i = 0; i < BlockOptions.size(); i++)
    {
        std::cout << "(x -> " << BlockOptions[i].GetSizeX()<< ", y -> " << BlockOptions[i].GetSizeY() << ") \n";
    }
}

bool update_layer_stability(Layer &layer, Layer &previous_layer)
{
    bool layer_stable = true;
    bool block_stable;
    for (auto& kv : layer.block_list)
    {
        block_stable = isBlockSupported(kv.second, previous_layer);
        if (!block_stable)
        {
            layer_stable = false;
        }
    }
    return layer_stable;
}

bool check_replacement(Block &block, Layer &layer, int x, int y)
//Checks if we can place the block at location x, y, with it only replacing 1x1 blocks.
{
    bool stable = false;
    for (int i = 0; i < block.GetSizeX(); ++i)
    {
        for (int j=0; j < block.GetSizeY(); ++j)
        {
            if (block.GetOccupied(i, j))
            {
                if (!layer.GetOccupancy(i + x, j + y))
                {
                    return false;
                }
                Block* old_block_ptr = layer.GetBlockPointer(i + x, j + y);
                
                if (old_block_ptr -> GetSizeX() != 1 || old_block_ptr -> GetSizeY() != 1 || old_block_ptr->irreplaceable)
                {
                    return false;
                }

                if (old_block_ptr -> isStable)
                {
                    stable = true;
                }
            }
        }
    }
    return stable;
}

Layer replace_block(Block new_block, Layer layer, int x, int y)
// Given a layer and a new block, return a NEW layer that has the new block placed at x,y
{
    // new_block is NOT passed by refrence, so we can use it without worrying about altering the orginal block. Change it's location to the provided location, and SET STABILE TO TRUE
    new_block.x = x;
    new_block.y = y;
    new_block.z = layer.z;
    new_block.isStable = true;
    new_block.irreplaceable = true;
    int added_block_id = layer.AddBlock(new_block);
    for (int i = 0; i < new_block.GetSizeX(); ++i)
    {
        for (int j=0; j < new_block.GetSizeY(); ++j)
        {
            if (new_block.GetOccupied(i, j))
            {
                layer.DeleteBlock(i + x, j + y);
                layer.SetVoxel(i + x, j + y, added_block_id);
            }
        }
    }
    return layer;
}

std::vector<Layer> find_stable_replacement(Layer &cur_layer, int x, int y, std::vector<Block> &possible_blocks)
// Finds all possible combinations that will result in stabilizing the unstable 1x1 block (located at x, y)
{
    std::vector<Layer> layer_possibilites;
    for (Block &block : possible_blocks)
    {
        // loop through all possible placements of the new block. The new block must be placed such that it covers x, y.
        for (int i = 0; i < block.GetSizeX(); ++i)
        {
            for (int j=0; j < block.GetSizeY(); ++j)
            {
                if (block.GetOccupied(i, j)) // make sure x, y would be covered if the block is placed at x-i, y-j
                {
                    if (check_replacement(block, cur_layer, x-i, y-j)) //See if this pacement is valid
                    {
                        layer_possibilites.push_back(replace_block(block, cur_layer, x-i, y-j));
                    }
                }
            }
        }
    }
    return layer_possibilites;
}

std::vector<Layer> stabilize_layer(Layer start_layer, std::vector<Block> &possible_shapes)
// Takes a layer and searches through all possible combinations of blocks form the possible shapes to find the one that 
// stabilizes the layer and minimizes cost.
{
    std::cout << "Start Stabilize for layer " << start_layer.z << std::endl;
    std::vector<Layer> testing_layers;
    std::vector<Layer> possible_layers;
    testing_layers.push_back(start_layer);
    Layer least_unstable_layer = start_layer;
    int min_unstable_blocks = start_layer.CountUnstableBlocks();
    int num_iters = 0; // number of iterations that have been run
    int max_iters = 100000; // max number of iterations to run stabilize for. If -1, no max.
    while (testing_layers.size() > 0 && (num_iters < max_iters || max_iters == -1))
    {
        Layer layer = testing_layers.back();
        testing_layers.pop_back();

        if (min_unstable_blocks != 0 && layer.CountUnstableBlocks() < min_unstable_blocks)
        {
            min_unstable_blocks = layer.CountUnstableBlocks();
            least_unstable_layer = layer;
        }

        bool move_to_next = false;
        // loop through all locations in the layer
        for (int x = 0; x < layer.size_x && !move_to_next; ++x)
        {
            for (int y = 0; y < layer.size_y && !move_to_next; ++y)
            {
                // if the entry is empty, skip
                if (layer.GetOccupancy(x, y) == false)
                {
                    continue;
                }

                // If the block is stable, skip
                if (layer.GetBlockPointer(x, y)->isStable == true)
                {
                    continue;
                }
                // Need to stabilize this Voxel:
                
                std::vector<Layer> new_layers = find_stable_replacement(layer, x, y, possible_shapes);
                for (Layer &new_layer: new_layers)
                {
                    testing_layers.push_back(new_layer);
                }
                move_to_next = true; 
            }
        }
        if (!move_to_next)
        {
            possible_layers.push_back(layer);
        }
        ++ num_iters;
    }
    if (possible_layers.size() == 0)
    {
        std::cout << "No stable layer found. The minimum number of unstable blocks is " << min_unstable_blocks << std::endl;
        std::vector<Layer> output_layer;
        output_layer.push_back(least_unstable_layer);
        return output_layer;
    }
    return possible_layers;
}

void prune_tree(std::deque<Layer> &layers, int keep_total)
// Prune the leaves of the search tree, keeping only keep_total leaves.
{
    // Sort the layers by cost, then only return best top keep_total.
    std::vector<std::pair<float, int> > layer_costs;
    layer_costs.resize(layers.size());
    for (int i =0; i < layers.size(); ++i)
    {
        layer_costs[i] = std::make_pair(layers[i].GetCost(), i);
    }
    std::sort(layer_costs.begin(), layer_costs.end());

    //create a new queue, only saving those at or below the threshold cost.
    std::deque<Layer> out_layers;

    for (int i = 0; i < keep_total; ++i)
    {
        out_layers.push_back(layers[layer_costs[i].second]);
    }
    //replace with swap.
    std::swap(layers, out_layers);
    // layers = out_layers;
}

Layer optimize_layer(std::vector<Layer> input_layers, std::vector<Block> &possible_shapes)
// Given a layer of 1x1 blocks, find the optimum aragnement of blocks from the possible shapes 
// that minimizes the cost function.
{
    //save size information.
    if (input_layers.size() == 0)
    {
        std::cout << "Optimize Layer was given an empty list as input" << std::endl;
        exit(1);
    }

    //create queue of potential layer configurations. Doing breadth first search
    //(FIFO) inorder to keep the depth of all potential maps relativley constant, so 
    // I can more easily prune the tree.
    std::deque<Layer> testing_layers;
    for (Layer &layer:input_layers)
    {
        testing_layers.push_back(layer);
    }

    // save the best layer and its cost. For now, just use the first input layer.
    Layer best_layer = input_layers[0];
    float best_cost = input_layers[0].GetCost();

    int test_num = 0; //numer of while loop iterations (for debuging)
    int search_depth = -1; //current depth of the breadth first search tree.
    while (testing_layers.size() > 0)
    {
        Layer layer = testing_layers.front(); // Get the current layer off the front of the queue.

        // If the depth of the layer is different form search_depth, then we've entered a new depth
        // of the search tree!
        if (layer.itercount != search_depth) 
        {
            search_depth = layer.itercount; //update serach depth.
            if (testing_layers.size() > 100) // If there are more than 1000 leaves, prune all by 100.
            {
                // std::cout << "pruning " << std::endl;
                prune_tree(testing_layers, 10);
                layer = testing_layers.front(); //after pruning, update the current layer.
            }
        }
        testing_layers.pop_front(); // remove the current layer (saved in layer) from the queue.

        // check if the layer cost is less than the best. If it is update the best.
        float layer_cost = layer.GetCost(); 
        if (layer_cost < best_cost)
        {
            best_layer = layer;
            best_cost = layer_cost;
        }

        // loop through all locations in the layer, trying new block shapes
        for (int i = layer.start_index; i < layer.size_x*layer.size_y; ++i)
        {
            //get x and y location in the layer
            int x = i%layer.size_x;
            int y = i/layer.size_x;

            // if the block is empty, skip.
            if (layer.GetOccupancy(x, y) == false)
            {
                continue;
            }

            Block* cur_voxel = layer.GetBlockPointer(x, y);

            // If the block is not 1x1, stable, and replaceable, skip:
            if (cur_voxel->GetSizeX() != 1 || cur_voxel->GetSizeY() != 1 || cur_voxel -> irreplaceable || !cur_voxel->isStable)
            {
                continue;
            }

            // Find possible solutions:
            std::vector<Layer> new_layers = find_stable_replacement(layer, x, y, possible_shapes);
            for (Layer &new_layer: new_layers)
            {
                new_layer.start_index = i; // nextime, start at position i in the layer
                ++new_layer.itercount; // update the depth of this layer in the search tree.
                testing_layers.push_back(new_layer); // Add the new possible solution to the search queue.
            }
            break; //move on to the new possibility.
        }
        ++test_num;
    }
    return best_layer;
}

void Merge(std::vector<std::vector<Block> > &block_lists, std::vector<Block> &possible_shapes, bool stabilize, std::function<bool(int)>shouldQuit)
// Merge blocks to form the combination that has the lowest cost and is stable.
{
    int max_x = 0;
    int max_y = 0;
    // Get the maximum x and y values for block structure.
    for (int i = 0; i<block_lists.size(); ++i)
    {
        for (int j = 0; j < block_lists[i].size(); ++j)
        {
            if (block_lists[i][j].x + block_lists[i][j].GetSizeX() > max_x)
            {
                max_x = block_lists[i][j].x + block_lists[i][j].GetSizeX();
            }
            if (block_lists[i][j].y + block_lists[i][j].GetSizeY() > max_y)
            {
                max_y = block_lists[i][j].y + block_lists[i][j].GetSizeY();
            }
        }
    }

    // Create the previous layer, the layer below the current layer.
    Layer previous_layer(max_x, max_y, 0, block_lists[0]);
    previous_layer.SetLayerStable(); // On the ground, so all of the blocks are stable.

    // loop through each layer.
    for (int z = 0; z < block_lists.size(); ++z)
    {
        wxYield();

        if(shouldQuit(z*100/(block_lists.size())))
        {
            std::cout << "Merge aborted" << std::endl;
            return;
        }
        // create the layer and check stability
        Layer cur_layer(max_x, max_y, z, block_lists[z]);
        bool is_layer_stable;
        if (stabilize)
        {
            is_layer_stable = update_layer_stability(cur_layer, previous_layer);
        }
        else
        {
            is_layer_stable = true;
            cur_layer.SetLayerStable();
        }

        // print intial total number of blocks
        // std::cout << "Layer " << z << " start vector size is " << cur_layer.GetBlockVector().size() << std::endl;

        std::vector<Layer> stable_layer_options;
        // if the layer isn't stable, stabilize it, and save all stabilizing configurations.
        if (!is_layer_stable)
        {
            stable_layer_options = stabilize_layer(cur_layer, possible_shapes);
        }
        else
        {
            stable_layer_options.push_back(cur_layer);
        }

        // optimize the layer.
        cur_layer = optimize_layer(stable_layer_options, possible_shapes);

        previous_layer = cur_layer; //update the previous layer.

        // update the blocklist with the optimized layer.
        block_lists[z] = cur_layer.GetBlockVector();

        //print final total number of blocks.
        // std::cout << "Layer " << z << " end vector size is " << block_lists[z].size() << std::endl;
    }
}

void optimize_layer_thread(std::vector<std::vector<Layer> > &stabilized_layers, std::vector<Layer> &optimized_layers, std::vector<int> &layers_left, std::vector<Block> possible_shapes)
{
    bool keep_going = true;
    while (keep_going)
    {
        mtx.lock();
        if (layers_left.size() > 0)
        {
            // std::cout << 100 - layers_left.size()*100/(stabilized_layers.size()) << std::endl;
            int layer_num = layers_left.back();
            layers_left.pop_back();
            mtx.unlock();
            optimized_layers[layer_num] = optimize_layer(stabilized_layers[layer_num], possible_shapes);
        }
        else
        {
            keep_going = false;
            mtx.unlock();
            break;
        }
    }
    
}


void MergeThreaded(std::vector<std::vector<Block> > &block_lists, std::vector<Block> &possible_shapes, bool stabilize, std::function<bool(int)> shouldQuit)
// Merge blocks to form the combination that has the lowest cost and is stable.
{
    const int n_threads = 8;
    int max_x = 0;
    int max_y = 0;

    // Get the maximum x and y values for block structure.
    for (int i = 0; i<block_lists.size(); ++i)
    {
        for (int j = 0; j < block_lists[i].size(); ++j)
        {
            if (block_lists[i][j].x + block_lists[i][j].GetSizeX()> max_x)
            {
                max_x = block_lists[i][j].x + block_lists[i][j].GetSizeX();
            }
            if (block_lists[i][j].y + block_lists[i][j].GetSizeY() > max_y)
            {
                max_y = block_lists[i][j].y + block_lists[i][j].GetSizeY();
            }
        }
    }

    // First we stabilize. Create a vector of the stabilized Layers. Since stabilization results in multiple possible layers,
    // This vector will be a vector of vectors of layers.

    std::vector<std::vector<Layer> > stabilized_layers;
    stabilized_layers.resize(block_lists.size());

    // Create the previous layer, the layer below the current layer.
    Layer previous_layer(max_x, max_y, 0, block_lists[0]);
    previous_layer.SetLayerStable(); // On the ground, so all of the blocks are stable.

    // loop through each layer.
    for (int z = 0; z < block_lists.size(); ++z)
    {
        wxYield();

        if(shouldQuit(z*100/(block_lists.size())))
        {
            std::cout << "Merge aborted" << std::endl;
            return;
        }
        // std::cout << z*100/(block_lists.size()) << std::endl;
        // create the layer and check stability
        Layer cur_layer(max_x, max_y, z, block_lists[z]);
        bool is_layer_stable;
        if (stabilize)
        {
            is_layer_stable = update_layer_stability(cur_layer, previous_layer);
        }
        else
        {
            is_layer_stable = true;
            cur_layer.SetLayerStable();
        }

        // // print intial total number of blocks
        // std::cout << "Layer " << z << " start vector size is " << cur_layer.GetBlockVector().size() << std::endl;

        // if the layer isn't stable, stabilize it, and save all stabilizing configurations.
        if (!is_layer_stable)
        {
            stabilized_layers[z] = stabilize_layer(cur_layer, possible_shapes);
        }
        else
        {
            stabilized_layers[z].push_back(cur_layer);
        }
        //update the previous layer. If there are more than one stable layers, it can be any of them.
        previous_layer = stabilized_layers[z][0]; 
    }

    // optimize the layers using the threads.
    WorkerThread threads[n_threads];

    std::vector<Layer> optimized_layers;
    optimized_layers.resize(stabilized_layers.size());

    std::vector<int> layers_left;
    layers_left.resize(stabilized_layers.size());
    for (int i = 0; i < stabilized_layers.size(); ++i)
    {
        layers_left[i] = stabilized_layers.size() - 1 - i;
    }

    for (int i = 0; i < n_threads - 1; ++i)
    {
        threads[i].Run([&, possible_shapes]{optimize_layer_thread(stabilized_layers, optimized_layers, layers_left, possible_shapes);});
    }

    for (int i = 0; i < n_threads - 1; ++i)
    {
        threads[i].Wait();
    }
    
    for (int z = 0; z < stabilized_layers.size(); ++z)
    {
        block_lists[z] = optimized_layers[z].GetBlockVector();
    }
}


bool NeverQuit(int in)
{
    return false;
}
// Old version of merge.
void Merge(std::vector<std::vector<Block> > &block_lists, std::vector<Block> &possible_shapes, bool stabilize)
// Merge blocks to form the combination that has the lowest cost and is stable.
{
    Merge(block_lists, possible_shapes, stabilize, NeverQuit);
}

void MergeThreaded(std::vector<std::vector<Block> > &block_lists, std::vector<Block> &possible_shapes, bool stabilize)
// Merge blocks to form the combination that has the lowest cost and is stable.
{
    MergeThreaded(block_lists, possible_shapes, stabilize, NeverQuit);
}
