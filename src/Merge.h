// Abraham
#ifndef MERGE_IS_INCLUDED
#define MERGE_IS_INCLUDED
#include <vector>
#include <iostream>
#include <functional>

#include "Block.h"
#include "Stability.h"
#include "cost.h"

// Merge header file

void Merge(std::vector<std::vector<Block> > &Layers, std::vector<Block> &BlockInfoVector, bool stabilize, std::function<bool(int)> shouldQuit);
void Merge(std::vector<std::vector<Block> > &Layers, std::vector<Block> &BlockInfoVector, bool stabilize); // old version of merge.
void MergeThreaded(std::vector<std::vector<Block> > &Layers, std::vector<Block> &BlockInfoVector, bool stabilize, std::function<bool(int)> shouldQuit);
void MergeThreaded(std::vector<std::vector<Block> > &Layers, std::vector<Block> &BlockInfoVector, bool stabilize); // old version of merge.
void test(std::vector<Block> BlockOptions);
#endif