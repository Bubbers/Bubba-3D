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
	Octree();
	Octree(float3 origin, float3 halfVector, int depth);
	~Octree();

	void insertTriangle(Triangle* t);
	void insertAll(std::vector<Triangle*> &triangles);

	bool hasChildren();
	void getChildren(std::vector<Octree*>* octs);
	void clearChildren();

	int getOctantContainingPoint(const float3& point);
	int getTriangleCount();
	int getTriangleCountRecursively();
	std::vector<Triangle*> *getTriangles();

	bool rayCastIntersectsAABB(float3 rayOrigin, float3 rayVector);
	void getTrianglesInsersectedByRayCast(float3 rayOrigin, float3 rayVector, std::vector<Triangle*> *tsp);

    AABB* getAABB();
	void setupAABB();

private:
	float3 getMaxXYZFromTwoPoints(float3 p1, float3 p2);
	float3 getMinXYZFromTwoPoints(float3 p1, float3 p2);

	void createChildren();

	void putTrianglesToList(std::vector<Triangle *> *triangleList);

	AABB aabb;

	Octree *children[8];
	std::vector<Triangle*> ts;
	int depth;

	float3 origin;
	float3 halfVector;

};


#endif
