#ifndef __COLLIDER_H__
#define __COLLIDER_H__

using namespace chag;

class GameObject;
class Triangle;
class Octree;

bool octreeOctreeIntersection(Octree *object1Octree, float4x4 *object1ModelMatrix, Octree *object2Octree,
                              float4x4 *object2ModelMatrix);
bool AabbAabbintersection(AABB *aabb1, AABB *aabb2);
bool capselIntersection(float3 capsel1pos,float capsel1radius,float3 capsel2pos,float capsel2radius);
AABB multiplyAABBWithModelMatrix(AABB *aabb, float4x4 modelMatrix);
bool triangleTriangleIntersection(Triangle *t1, Triangle *t2);
bool isTrianglesIntersecting(Octree *object1Octree, float4x4 *object1ModelMatrix, Octree *object2Octree,
							 float4x4 *object2ModelMatrix);

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