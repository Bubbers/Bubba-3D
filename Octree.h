#ifndef __Octree_h_
#define __Octree_h_

#include <float3.h>
#include "Triangle.h"
#include <vector>
#include <set>
#include <ctime>
#include <chrono>
#include <math.h>

using namespace std;
using namespace chag;
using namespace chrono;

const int MAX_DEPTH = 3;
const int MAX_CHILDREN = 64;

class Octree {
public:
	Octree(float3 origin, float3 halfVector, int depth);
	~Octree(void);

	bool hasChildren(void);
	void insert(Triangle* t);

	float3 origin;
	float3 halfVector;
	

	void getChildren(std::vector<Octree>* octs);
	bool isFull();
	int getOctantContainingPoint(const float3& point);
	int getChildCount();
	int getCount();
	bool intersect(float3 rayOrigin, float3 rayVector);
	void getGeometry(float3 rayOrigin, float3 rayVector, std::vector<Triangle*> *tsp);

private:
	
	void addTriangle(Triangle* t);

	Octree *children[8];
	std::vector<Triangle*> ts;
	int depth;
};


#endif