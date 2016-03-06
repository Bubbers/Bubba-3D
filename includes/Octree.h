#ifndef __Octree_h_
#define __Octree_h_

#include <float3.h>
#include <vector>
#include <set>
#include <ctime>
#include <math.h>
#include "AABB2.h"

using namespace std;
using namespace chag;

class Triangle;

const int MAX_DEPTH = 8;
const int MAX_CHILDREN = 16;

class Octree {
public:
    /**
     * Initiates the Octree with default values for origin and halfVector.
     * Should NOT be used unless you really want a small Octree.
     * // TODO(Bubbad) Make Octree resize after insertions to make this constructor useful
     */
    Octree();
    /**
     * Initiates a Octree that can contain triangles that
     * are at within the cube of size 2*halfVector that
     * is centered around the specified origin.
     *
     * @param origin The center position of the Octree
     * @param halfVector Half the length of the surrounding cube
     */
    Octree(float3 origin, float3 halfVector);

    ~Octree();

    void insertTriangle(Triangle* t);
    void insertAll(std::vector<Triangle*> &triangles);

    /**
     * return True if the Octree is recursed and have triangles in deeper levels, false otherwise
     */
    bool hasChildren();
    /**
     * Fills the specified list with all the Octrees child Octrees
     *
     * @param octs List to be filled
     */
    void getChildren(std::vector<Octree*>* octs);
    /**
     * Removes all the Octrees children and its triangles
     */
    void clearChildren();

    int getOctantContainingPoint(const float3& point);
    int getTriangleCount();
    /**
     * @return The triangle count of this Octree + the triangle count of all its child Octrees
     */
    int getTriangleCountRecursively();
    /**
     * @return A list of the triangles contained in this level of the Octree
     */
    std::vector<Triangle*> *getTriangles();

    /**
     * @return Recursivly searches the Octrees child Octrees and count how many sub trees there are.
     */
    int getNumberOfSubTrees();


    /**
     * Recursively searches the Octree for triangles hit by the ray cast
     *
     * @param triangleList A list to be filled with all intersected triangles
     */
    void getTrianglesInsersectedByRayCast(float3 rayOrigin, float3 rayVector, std::vector<Triangle*> *triangleList);

    /**
     * NOTE: The AABB is not transformed.
     */
    AABB* getAABB();

private:
    Octree(float3 origin, float3 halfVector, int depth);

    void setupAABB(float3 origin, float3 halfVector);

    /**
     * Allocates memory for 8 children.
     */
    void createChildren();

    /**
     * Combines both vectors into one. If the comparator is true for an element of the vector,
     * it takes the element from p1, else p2.
     * Useful for eg creating a vector containing the max or min elements of two vectors.
     */
    float3 combineTwoPointsByComparator(float3 p1, float3 p2, std::function<bool(float, float)> comparator);
    
    bool rayCastIntersectsAABB(float3 rayOrigin, float3 rayVector);
    void putTrianglesToList(std::vector<Triangle *> *triangleList);

    Octree *children[8];
    std::vector<Triangle*> ts;
    int depth;

    float3 origin;
    float3 halfVector;

    AABB aabb;
};


#endif
