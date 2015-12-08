#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <GameObject.h>
#include <float4.h>
#include <float3.h>
#include <Triangle.h>
#include <Octree.h>

using namespace chag;

bool triangleTriangleIntersection(Triangle* t1, Triangle* t2);
bool octreeOctreeIntersection(Octree *object1Octree, float4x4 *object1ModelMatrix, Octree *object2Octree,
							  float4x4 *object2ModelMatrix);
bool AabbAabbintersection(AABB* aabb1, AABB* aabb2);
bool triangleAabbIntersection(AABB* aabb, Triangle* triangle);
bool lineSegmentAabbIntersection(AABB* aabb, float3 linesegment);

class Collider
{
public:
	Collider(Octree* tree);
	~Collider();
	void addGameObject(GameObject*gameObject);
	void insertAll();
	float rayIntersection(float3 rayOrigin, float3 rayVec);

private:
	Octree* tree;
	std::vector<Triangle*> ts;
	AABB aabb_coll;
};

#endif //__COLLIDER_H__