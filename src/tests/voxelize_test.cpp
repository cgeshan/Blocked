#include "../Voxelize.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include "../Block.h"


int main(int argc,char *argv[])
{
    if(argc<2) return 1;
    float voxel_size=1.0;
    if(argc>2) voxel_size = std::stof(argv[2]);

    LatticeBlock L;
    L.VoxelizeSTL(argv[1], voxel_size, voxel_size*0.51);

    printf("(%d, %d, %d)\n", L.nx, L.ny, L.nz);
    FILE* file = fopen("grid.txt", "w");
    if (file == nullptr) {
        printf("Error: Could not open file for writing voxel centers\n");
        return 1;
    }
    
    unsigned int counter1=0, counter2=0;   

    for(int k=0; k<L.nz; k++)
    {
        for(int i=0; i<L.nx; i++)
        {
            for(int j=0; j<L.ny; j++)
            {
                int filled = L.GetElement(i,j,k).filled;
                if(filled==-1) return 1;
                fprintf(file, "%d ", filled>0);
                counter1 += filled>0;
            }
        fprintf(file, "\n");
        }
        fprintf(file, "\n");
        counter2 += L.layers[k].size();
    }
    fclose(file);
    if(counter1!=counter2) return 1;
    return 0;
}
