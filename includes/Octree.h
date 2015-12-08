#ifndef __Octree_h_
#define __Octree_h_

#include <float3.h>
#include "Triangle.h"
#include <vector>
#include <set>
#include <ctime>
#include <math.h>
#include "AABB2.h"

using namespace std;
using namespace chag;

const int MAX_DEPTH = 8;
const int MAX_CHILDREN = 32;

class Octree {
public:
  Octree();
	Octree(float3 origin, float3 halfVector, int depth);
	~Octree(void);

	bool hasChildren(void);
	void insert(Triangle* t);
	void insertAll(std::vector<Triangle*> &triangles);

	float3 origin;
	float3 halfVector;
	

	void getChildren(std::vector<Octree*>* octs);
    std::vector<Triangle*> *getTriangles();

	bool isFull();
	int getOctantContainingPoint(const float3& point);
	int getChildCount();
	int getCount();
	bool intersect(float3 rayOrigin, float3 rayVector);
	void getGeometry(float3 rayOrigin, float3 rayVector, std::vector<Triangle*> *tsp);
    AABB* getAABB();

private:
	
	void addTriangle(Triangle* t);
	AABB aabb;

	Octree *children[8];
	std::vector<Triangle*> ts;
	int depth;
};


#endif
