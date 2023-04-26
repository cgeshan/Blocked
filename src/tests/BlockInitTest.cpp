#include <iostream>
#include "../Block.h"

int main(void)
{
    Block b(1, 2, 3, 2, 3, 1.00);

    if(b.x == 1 && b.y == 2 && b.z == 3 && b.GetSizeX() == 2 && b.GetSizeY() == 3 && b.cost == 1.00)
    {
        exit(0);
    }
    else
    {
        exit(1);
    }
}