#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <GameObject.h>
#include <float4.h>
#include <float3.h>
#include <Triangle.h>
#include <Octree.h>

using namespace chag;

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