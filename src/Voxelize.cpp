/* Author: Pouya */
#include <math.h>
#include <algorithm>
#include <queue>
#include <iostream>
#include <stdio.h>
#include <functional>
#include "Voxelize.h"
#include "Block.h"

float GetRandUniform(void)
{
    return (float)rand() / ((float)RAND_MAX / 2) - 1.0;
}
/* Start of Helper Functions */
Vec3f Cross(const Vec3f &v1, const Vec3f &v2)
{
    Vec3f v;
    v.x = v1.y*v2.z - v1.z*v2.y;
    v.y = v1.z*v2.x - v1.x*v2.z;
    v.z = v1.x*v2.y - v1.y*v2.x;
    return v;
}
float Dot(const Vec3f &v1, const Vec3f &v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
float SignedVolume(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Vec3f &d)
{
    return Dot(Cross(b-a, c-a), d-a)/6.0;
}
bool LineHitsTriangle(Vec3f q1, Vec3f q2, const Triangle &tri)
{
    bool debug = false;
    do
    {
    float a,b;
    debug = false;
    a = SignedVolume(q1, tri.p1, tri.p2, tri.p3);
    b = SignedVolume(q2, tri.p1, tri.p2, tri.p3);
    if (a==0) {debug=true;}
    if (b==0) {debug=true;}
    if(a*b>0) return false;
    float c,d,e;
    c = SignedVolume(q1, q2, tri.p1, tri.p2);
    d = SignedVolume(q1, q2, tri.p2, tri.p3);
    e = SignedVolume(q1, q2, tri.p3, tri.p1);
    if (c == 0) {debug=true;}
    if (d == 0) {debug=true;}
    if (e == 0) {debug=true;}
    if(c>0 && d>0 && e>0) return true;
    if(c<0 && d<0 && e<0) return true;
    if (debug)
    {
        q1 = q1 + Vec3f(GetRandUniform()*1e-3, GetRandUniform()*1e-3, GetRandUniform()*1e-3);
        q2 = q2 + Vec3f(GetRandUniform()*1e-3, GetRandUniform()*1e-3, GetRandUniform()*1e-3);
    }
    } 
    while (debug);
    return false;
    
    
}

void GetBox(const Triangle &T, Vec3f &min, Vec3f &max)
{
    min.x = std::min({T.p1.x, T.p2.x, T.p3.x});
    min.y = std::min({T.p1.y, T.p2.y, T.p3.y});
    min.z = std::min({T.p1.z, T.p2.z, T.p3.z});
    max.x = std::max({T.p1.x, T.p2.x, T.p3.x});
    max.y = std::max({T.p1.y, T.p2.y, T.p3.y});
    max.z = std::max({T.p1.z, T.p2.z, T.p3.z});
}
void UpdateBox(const Triangle &T, Vec3f &min, Vec3f &max)
{
    min.x = std::min({min.x, T.p1.x, T.p2.x, T.p3.x});
    min.y = std::min({min.y, T.p1.y, T.p2.y, T.p3.y});
    min.z = std::min({min.z, T.p1.z, T.p2.z, T.p3.z});
    max.x = std::max({max.x, T.p1.x, T.p2.x, T.p3.x});
    max.y = std::max({max.y, T.p1.y, T.p2.y, T.p3.y});
    max.z = std::max({max.z, T.p1.z, T.p2.z, T.p3.z});
}

bool CloseEnoughToLine(const Vec3f pos, const Vec3f p1, const Vec3f p2, float tol)
{
    Vec3f p1_p2 = p2 - p1;
    Vec3f e = p1_p2.unit();
    Vec3f p1_pos = pos - p1;
    float p1_p2_length = p1_p2.Length();
    float p1_proj_length = Dot(p1_pos, e);
    if(p1_proj_length<0)
    {
        if((pos-p1).Length()>tol) return false; else return true;
    }
    else if(p1_proj_length>p1_p2_length)
    {
        if((pos-p2).Length()>tol) return false; else return true;
    }
    else
    {
        Vec3f proj = p1 + e*p1_proj_length;
        float d = (pos-proj).Length();
        if(d>tol) return false; else return true;
    }

}
bool CloseEnoughToTriangle(const Vec3f pos, const Triangle &tri, float tol)
{
    Vec3f d = pos - tri.p1;
    // if too far from triangle's plane:
    float distance = Dot(d, tri.nom);
    if(std::fabs(distance)>tol) return false;

    // checking if the projection onto the plane is inside the triangle
    Vec3f proj = pos - tri.nom*distance;

    Vec3f pp1, pp2, pp3, c1, c2, c3;
    pp1 = tri.p1 - proj;
    pp2 = tri.p2 - proj;
    pp3 = tri.p3 - proj;

    c1 = Cross(pp1, pp2);
    c2 = Cross(pp2, pp3);
    c3 = Cross(pp3, pp1);

    float fc1, fc2, fc3;
    fc1 = Dot(c1, c2);
    fc2 = Dot(c2, c3);
    fc3 = Dot(c3, c1);

    if(fc1>0 && fc2>0 && fc3>0) return true;
    if(fc1<0 && fc2<0 && fc3<0) return true;

    // if the projection is outside the triangle:
    if(CloseEnoughToLine(pos, tri.p1, tri.p2, tol)) return true;
    if(CloseEnoughToLine(pos, tri.p2, tri.p3, tol)) return true;
    if(CloseEnoughToLine(pos, tri.p1, tri.p3, tol)) return true;

    return false;
}

bool ReadBinarySTL(std::vector <Triangle> &tris, Vec3f &min, Vec3f &max, const char fName[])
{
	FILE *ifp=fopen(fName,"rb");
	if(nullptr!=ifp)
	{
		unsigned char buf[80];

		fread(buf,1,80,ifp);

		fread(buf,1,4,ifp);
		unsigned int nPlg=*(uint32_t *)buf;

        bool first = true;

		for(int i=0; i<nPlg; ++i)
		{
			if(50==fread(buf,1,50,ifp))
			{
				float *info=(float *)buf;
                Triangle tri;
                tri.nom = Vec3f(info[0], info[1], info[2]);
                tri.p1 = Vec3f(info[3], info[4], info[5]);
                tri.p2 = Vec3f(info[6], info[7], info[8]);
                tri.p3 = Vec3f(info[9], info[10], info[11]);
                tris.push_back(tri);
                if (first)
                {
                    GetBox(tri, min, max);
                    first = false;
                }
                else
                {
                    UpdateBox(tri, min, max);
                }
			}
			else
			{
				break;
			}
		}

		fclose(ifp);
		return true;
	}
	return false;
}
void Loop2d(int x0, int x1, int y0, int y1, std::function<void(int, int)> func)
{
    for(int i=x0; i<x1; i++)
    {
        for(int j=y0; j<y1; j++)
        {
            func(i, j);
        }
    }
}
void Loop3d(int x0, int x1, int y0, int y1, int z0, int z1, std::function<void(int, int, int)> func)
{
    for(int i=x0; i<x1; i++)
    {
        for(int j=y0; j<y1; j++)
        {
            for(int k=z0; k<z1; k++)
            {
                func(i, j, k);
            }
        }
    }
}
/* End of Helper Functions */

Vec3f::Vec3f(){}
Vec3f::Vec3f(float x0, float y0, float z0)
{
    x = x0;
    y = y0;
    z = z0;
}
Vec3f Vec3f::operator-(const Vec3f v2) const
{
    return Vec3f(x-v2.x, y-v2.y, z-v2.z);
}
Vec3f Vec3f::operator+(const Vec3f v2) const
{
    return Vec3f(x+v2.x, y+v2.y, z+v2.z);
}
Vec3f Vec3f::operator*(float c) const
{
    return Vec3f(c*x, c*y, c*z);
}
Vec3f Vec3f::operator/(float c) const
{
    return Vec3f(x/c, y/c, z/c);
}
float Vec3f::Length(void) const
{
    return sqrt(x*x + y*y + z*z);
}
Vec3f Vec3f::unit(void) const
{
    return *this/Length();
}

Vec3i::Vec3i(){}
Vec3i::Vec3i(int x0, int y0, int z0)
{
    x = x0;
    y = y0;
    z = z0;
}
Vec3i Vec3i::operator-(const Vec3i v2) const
{
    return Vec3i(x-v2.x, y-v2.y, z-v2.z);
}
Vec3i Vec3i::operator+(const Vec3i v2) const
{
    return Vec3i(x+v2.x, y+v2.y, z+v2.z);
}

LatticeBlock::Element::Element()
{
    trisptr.clear();
    filled = -1;
}

LatticeBlock::LatticeBlock()
{
    nx=0;
    ny=0;
    nz=0;
    elements.clear();
    triangles.clear();
    layers.clear();
    voxel_size=1.0;
    voxelize_tol=sqrt(2.0)/2.0;
}
void LatticeBlock::Create(Vec3f minf, Vec3f maxf, float voxel_size, float voxelize_tol)
{
    nx = std::ceil((maxf.x-minf.x)/voxel_size);
    ny = std::ceil((maxf.y-minf.y)/voxel_size);
    nz = std::ceil((maxf.z-minf.z)/voxel_size);
    elements.resize(nx*ny*nz);
    this->voxel_size = voxel_size;
    this->voxelize_tol = voxelize_tol;
    Vec3f offset = Vec3f( float(nx)*voxel_size, float(ny)*voxel_size, float(nz)*voxel_size ) - (maxf-minf);
    min = minf - offset/2.0;
    max = maxf + offset/2.0;
    layers.resize(nz);
    for(int k=0; k<nz; k++)
    {
        layers[k].clear();
    }
}
Vec3f LatticeBlock::GetFloatPosition(int x, int y, int z) const
{
    return Vec3f(
        min.x + voxel_size*(float)x + voxel_size*0.5,
        min.y + voxel_size*(float)y + voxel_size*0.5,
        min.z + voxel_size*(float)z + voxel_size*0.5
        );
}
Vec3f LatticeBlock::GetFloatPosition(Vec3i pos) const
{
    return GetFloatPosition(pos.x, pos.y, pos.z);
}
Vec3i LatticeBlock::GetIndex(Vec3f pos) const
{
    int x, y, z;
    x = std::floor((pos.x-min.x)/voxel_size);
    y = std::floor((pos.y-min.y)/voxel_size);
    z = std::floor((pos.z-min.z)/voxel_size);
    if (x == nx) x--; 
    if (y == ny) y--; 
    if (z == nz) z--; 
    return Vec3i(x,y,z);
}
bool LatticeBlock::IsInRange(int x, int y, int z) const
{
    if(x<0 || x>=nx || y<0 || y>=ny || z<0 || z>=nz) return false;
    return true;
}
bool LatticeBlock::IsInRange(Vec3i pos) const
{
    return IsInRange(pos.x, pos.y, pos.z);
}

LatticeBlock::Element& LatticeBlock::GetElement(int x, int y, int z)
{
    return elements[z*nx*ny + y*nx + x];
}
LatticeBlock::Element& LatticeBlock::GetElement(Vec3i pos)
{
    return GetElement(pos.x, pos.y, pos.z);
}

void LatticeBlock::VoxelizeTriangle(const Triangle &tri)
{
    Vec3f min, max;
    GetBox(tri, min, max);
    Vec3i start = GetIndex(min);
    Vec3i end = GetIndex(max) + Vec3i(1,1,1);
    auto CheckOneElement = [&](int i, int j, int k)
    {
        if(CloseEnoughToTriangle(GetFloatPosition(i,j,k), tri, voxelize_tol))
        {
            if(GetElement(i,j,k).filled<1)
            {
                CheckElement(Vec3i(i,j,k),1);
            }
        }

    };
    Loop3d(start.x, end.x, start.y, end.y, start.z, end.z, CheckOneElement);
}
void LatticeBlock::VoxelizeShell(void)
{
    for( auto &tri : triangles)
    {
        VoxelizeTriangle(tri);
    }
}

void LatticeBlock::CheckElement(Vec3i pos, int mode)
{
    auto &element = GetElement(pos);
    if(element.filled!=-1) printf("Refilling (%d,%d,%d) as %d from %d!\n", pos.x, pos.y, pos.z, element.filled, mode);
    element.filled = mode;
    if(mode==1)
    {
        layers[pos.z].push_back(Block(pos.x, pos.y, pos.z, 1, 1, 1.0));
    }

}
void LatticeBlock::CheckAndFlood(int x, int y, int z)
{
    if(GetElement(x,y,z).filled!=-1) return;
    unsigned int hit=0;
    Vec3f q1, q2;
    q1 = GetFloatPosition(x,y,z);
    q2 = GetFloatPosition(-1, -1, -1);
    for(auto &tri : triangles)
    {
        if(LineHitsTriangle(q1, q2, tri)) hit++;
    }
    int mode;
    if(hit%2==0) mode=0;
    else mode=1;
    Flood(Vec3i(x,y,z), mode);
}
void LatticeBlock::Flood(Vec3i pos, int mode)
{
    std::queue <Vec3i> ToCheck;
    ToCheck.push(pos);
    CheckElement(pos, mode);
    while(!ToCheck.empty())
    {
        Vec3i current = ToCheck.front();ToCheck.pop();
        int i0=current.x, j0=current.y, k0 = current.z;
        Vec3i a1(i0-1, j0, k0), a2(i0+1, j0, k0), a3(i0, j0-1, k0), a4(i0, j0+1, k0), a5(i0, j0, k0-1), a6(i0, j0, k0+1);
        if(IsInRange(a1) && GetElement(a1).filled==-1) {CheckElement(a1,mode); ToCheck.push(a1);}
        if(IsInRange(a2) && GetElement(a2).filled==-1) {CheckElement(a2,mode); ToCheck.push(a2);}
        if(IsInRange(a3) && GetElement(a3).filled==-1) {CheckElement(a3,mode); ToCheck.push(a3);}
        if(IsInRange(a4) && GetElement(a4).filled==-1) {CheckElement(a4,mode); ToCheck.push(a4);}
        if(IsInRange(a5) && GetElement(a5).filled==-1) {CheckElement(a5,mode); ToCheck.push(a5);}
        if(IsInRange(a6) && GetElement(a6).filled==-1) {CheckElement(a6,mode); ToCheck.push(a6);}
        
    }
}
void LatticeBlock::VoxelizeVolume(void)
{
    auto caf = [&](int i, int j, int k){CheckAndFlood(i,j,k);};
    Loop3d(0, nx, 0, ny, 0, nz, caf);
}

void LatticeBlock::VoxelizeSTL(const char filename[], float voxel_size, float voxelize_tol_in)
{
    if(!ReadBinarySTL(triangles, min, max, filename)) return;
    // printf("STL imported, min: (%g,%g,%g), max: (%g,%g,%g), %d triangles\n",
    // min.x, min.y, min.z, max.x, max.y, max.z, triangles.size());
    Create(min, max, voxel_size, voxelize_tol_in);
    // printf("grid (%d,%d,%d)\n", nx, ny, nz);
    VoxelizeShell();
    VoxelizeVolume();
}

void LatticeBlock::VoxelizeSTL(const char filename[], int num_voxels)
{
    if(!ReadBinarySTL(triangles, min, max, filename)) return;
    // printf("STL imported, min: (%g,%g,%g), max: (%g,%g,%g), %d triangles\n",
    // min.x, min.y, min.z, max.x, max.y, max.z, triangles.size());
    Vec3f range = max - min;
    float maxdim = std::max({range.x, range.y, range.z});
    float voxel_size = maxdim/float(num_voxels);
    Create(min, max, voxel_size, voxel_size*0.51);
    // printf("grid (%d,%d,%d)\n", nx, ny, nz);
    VoxelizeShell();
    // printf("shell done\n");
    VoxelizeVolume();
}

std::vector<std::vector<Block> > STLToBlocks(const char filename[], float voxel_size)
{
    LatticeBlock L;
    L.VoxelizeSTL(filename, voxel_size, voxel_size*0.51);
    return L.layers;
}

std::vector<std::vector<Block> > STLToBlocksInt(const char filename[], int num_voxels)
{
    LatticeBlock L;
    L.VoxelizeSTL(filename, num_voxels);
    return L.layers;
}