// Abraham
#ifndef HELPERFUNCTIONS_IS_INCLUDED
#define HELPERFUNCTIONS_IS_INCLUDED

#include <vector>

#include "Block.h"
#include "Layer.h"

std::vector<std::vector<Block> > create_test_structure(void);
void printBlocks(std::vector<std::vector<Block> > layers, bool print_stability);
void printStabilities(std::vector<std::vector<Block> > layers);
void PrintLayerStability(Layer layer);
void PrintLayer(Layer layer);
#endif