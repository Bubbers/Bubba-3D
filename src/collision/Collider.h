#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include <Mesh.h>
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
	void addMesh(Mesh* model, float4x4 modelMatrix);
	void insertAll();
	float rayIntersection(float3 rayOrigin, float3 rayVec);

private:
	Octree* tree;
	std::vector<Triangle*> ts;
	AABB aabb_coll;
};

#endif //__COLLIDER_H__