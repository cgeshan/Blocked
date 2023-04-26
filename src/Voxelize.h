/* Author: Pouya */
#ifndef VOXELIZE_H_IS_INCLUDED
#define VOXELIZE_H_IS_INCLUDED

#include <vector>
#include <unordered_map>
#include "Block.h"

class Vec3f
{
public:
    float x=0, y=0, z=0;
    Vec3f();
    Vec3f(float x0, float y0, float z0);
    Vec3f operator-(const Vec3f v2) const;
    Vec3f operator+(const Vec3f v2) const;
    Vec3f operator*(float c) const;
    Vec3f operator/(float c) const;
    float Length(void) const;
    Vec3f unit(void) const;
};
struct Triangle
{
public:
    Vec3f p1, p2, p3;
    Vec3f nom;
};
class Vec3i
{
public:
    int x=-1, y=-1, z=-1;
    Vec3i();
    Vec3i(int x0, int y0, int z0);
    Vec3i operator-(const Vec3i v2) const;
    Vec3i operator+(const Vec3i v2) const;
};

class LatticeBlock
{
public:
    class Element
    {
    public:
        int filled=-1; // -1 unknown, 0 empty, 1 known
        std::vector <const Triangle*> trisptr;
        Element();
    };

    int nx, ny, nz;
    std::vector <Element> elements;
    std::vector <Triangle> triangles;
    std::vector <std::vector <Block> > layers;

    Vec3f min, max; // the bounding box of the whole structure
    float voxel_size, voxelize_tol;

    LatticeBlock();
    void Create(Vec3f minf, Vec3f maxf, float voxel_size, float voxelize_tol);

    Vec3f GetFloatPosition(int x, int y, int z) const;
    Vec3f GetFloatPosition(Vec3i pos) const;
    Vec3i GetIndex(Vec3f pos) const;
    bool IsInRange(int x, int y, int z) const;
    bool IsInRange(Vec3i pos) const;

    Element &GetElement(int x, int y, int z);
    Element &GetElement(Vec3i pos);

    void VoxelizeTriangle(const Triangle &tri);
    void VoxelizeShell(void);

    void CheckElement(Vec3i pos, int mode);
    void CheckAndFlood(int x, int y, int z);
    void Flood(Vec3i pos, int mode);
    void VoxelizeVolume(void);

    void VoxelizeSTL(const char filename[], float voxel_size, float voxelize_tol);
    void VoxelizeSTL(const char filename[], int num_voxels);
};

std::vector<std::vector<Block> > STLToBlocks(const char filename[], float voxel_size);
std::vector<std::vector<Block> > STLToBlocksInt(const char filename[], int num_voxels);

#endif