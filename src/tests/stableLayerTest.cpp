#include "../Stability.h"
#include "../Block.h"
#include "../Layer.h"
#include "../HelperFunctions.h"

int main(void)
{ 
    std::vector <Block> emtpyLayer; 

    std::vector<std::vector<Block>> blocks = create_test_structure();
    Layer topLayerTestStructure(4, 2, 3, blocks[2]);
    
    if (isLayerStable(blocks[2], topLayerTestStructure) == true)
    {
        return 0; // pass - layer should return stable if previous layer equals current 
    }
    else
    {
        return 1; // fail 
    } 
}