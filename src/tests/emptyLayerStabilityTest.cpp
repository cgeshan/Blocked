#include "../Stability.h"
#include "../Block.h"
#include "../Layer.h"
#include "../HelperFunctions.h"

int main(void)
{ 
    std::vector <Block> emtpyLayer; 

    std::vector<std::vector<Block>> blocks = create_test_structure();
    Layer topLayerTestStructure(4, 2, 3, blocks[2]);
    
    if (isLayerStable(emtpyLayer, topLayerTestStructure) == true)
    {
        return 0; // pass - empty layer should return stable if blocks below are stable
    }
    else
    {
        return 1; // fail
    } 
}