// Charlotte Avra
#ifndef STABILITY_IS_INCLUDED
#define STABILITY_H_IS_INCLUDED
#include <vector>

#include "Layer.h"

bool isBlockSupported(Block& curBlock, Layer prevLayer);
bool isLayerStable(std::vector <Block> curLayer, Layer prevLayer);
#endif