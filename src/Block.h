#ifndef BLOCK_IS_INCLUDED
#define BLOCK_IS_INCLUDED

#include <vector>

// Block class 

class Block
{
public:
	int x, y; // location of the bottom corner of the block
	int z; // layer height
	int id = -1; // ID, for use with the Layer Class
	bool isStable = false; //is  the block stable
	bool irreplaceable = false; // can the block be replaced. Used in merge.
	float cost = 0;

	Block(int x, int y, int z, int size_x, int size_y, float cost);
	Block(int size_x, int size_y, float cost);
	Block();
    ~Block();

	void SetBlockElementEmpty(int x, int y); // Sets compoenent elements entry to false.
	bool GetOccupied(int x, int y); // returns component elments value.
	bool IsRect(void);
	int GetSizeX(void);
	int GetSizeY(void);

private:
	int size_x = 0; int size_y = 0; // outline size
	// Determines whether a voxel inside of the block is occupied. Used for L shapes
	std::vector<bool> component_elements; 
};

#endif