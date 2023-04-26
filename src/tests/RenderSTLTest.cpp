#include <vector>
#include <iostream>

void PushXYZ(std::vector <float> &vtx, float x, float y, float z)
{
	vtx.push_back(x);
	vtx.push_back(y);
	vtx.push_back(z);
}

void PushRGBA(std::vector <float> &col, float r, float g, float b, float a)
{
	col.push_back(r);
	col.push_back(g);
	col.push_back(b);
	col.push_back(a);
}

int main(void)
{
    std::vector <float> vtx, nom, col;

    vtx.clear();
	nom.clear();
	col.clear();
    
    std::cout << "Reading STL...\n";
    FILE *ifp = fopen("../../data/cone.stl", "rb");
	if(nullptr != ifp)
	{
		unsigned char buf[80];

		fread(buf, 1, 80, ifp);

		fread(buf, 1, 4, ifp);
		unsigned int nPlg = *(uint32_t *)buf;

		printf("%d\n", nPlg);

		for(int i = 0; i < nPlg; i++)
		{
			if(50 == fread(buf, 1, 50, ifp))
			{
				float *tri = (float *)buf;
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);

				PushXYZ(vtx, tri[3], tri[4], tri[5]);
				PushXYZ(vtx, tri[6], tri[7], tri[8]);
				PushXYZ(vtx, tri[9], tri[10], tri[11]);
			}
			else
			{
				break;
			}
		}
        std::cout << "STL imported, closing file\n";
		fclose(ifp);
	}

    if(vtx.size() < 3)
    {
        exit(1);
    }
    if(nom.size() < 3)
    {
        exit(1);
    }
    return 0;
}