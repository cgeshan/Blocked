// Charlotte Avra

#include <vector>

#include "Stability.h"
#include "Layer.h"

bool isBlockSupported(Block& curBlock, Layer prevLayer)
{
	/*
	This function determines if a single block is supported by the previous layer.
	If there exists at least one stud in the previous layer that is directly underneath at least one stud in the current block, the block is supported.
	*/

	// get block shape 
	int size_x = curBlock.GetSizeX();
	int size_y = curBlock.GetSizeY();

	// define empty stud location vectors
	std::vector<int> x_studLoc; 
	std::vector<int> y_studLoc;

	// define initial stud location 
	x_studLoc.push_back(curBlock.x);
	y_studLoc.push_back(curBlock.y);

	// define all other stud locations
	for (int i = 1; i < size_x; i++)
	{
		x_studLoc.push_back(x_studLoc[i - 1] + 1); // next stud location in x
	}
	for (int j = 1; j < size_y; j++)
	{
		y_studLoc.push_back(y_studLoc[j - 1] + 1); // next stud location in y
	}

	// get occupancy (iterate through all stud locations and check occupancy of a stud in the previous layer)
	// at least one of the block's studs must be supported by a stud in the previous layer
	for (int i = 0; i < size_x; i++)
	{
		for (int j = 0; j < size_y; j++)
		{
			if (prevLayer.GetOccupancy(x_studLoc[i], y_studLoc[j]) == true && curBlock.GetOccupied(i, j) && prevLayer.GetBlockPointer(x_studLoc[i], y_studLoc[j])->isStable)
			{
				curBlock.isStable = true;
				return true; // done because we found at least one of current block's studs is supported in the previous layer
			}
		}
	}
	curBlock.isStable = false;
	return false;
}

bool isLayerStable(std::vector <Block> curLayer, Layer prevLayer)
{
	/*
	This function determines whether a layer in the merged model is stable given the previous layers are stable.
	If a single block is not supported, the entire layer is defined as unstable.

	Args:
		curLayer: a vector of blocks of the same 'z' value of which we want to know if it is stable
		prevLayer: a bool array of the previous layer where false indicates a missing stud and true indicates a stud exists 

	Returns:
		bool True if layer is stable
		bool False if layer is not stable
	*/

	for (int i = 0; i < curLayer.size(); i++) // iterate through all blocks in current layer
	{	
		if (isBlockSupported(curLayer[i], prevLayer) == false)
		{
			return false; // if one block is not supported, the layer is unstable  
		}
	}
	return true; // all blocks are supported so layer is stable 
}

