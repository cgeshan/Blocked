#include "../Stability.h"
#include "../Block.h"
#include "../Layer.h"
#include "../HelperFunctions.h"

int main(void)
{ 
    std::vector <Block> emptyLayerVector; 
    Layer emptyLayer(4, 2, 3, emptyLayerVector);

    std::vector<std::vector<Block>> blocks = create_test_structure();
    
    if (isLayerStable(blocks[2], emptyLayer) == false)
    {
        return 0; // pass - layer should return unstable if there are no blocks in previous layer
    }
    else
    {
        return 1; // fail 
    } 
}