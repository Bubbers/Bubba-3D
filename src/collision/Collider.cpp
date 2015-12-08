#include <GameObject.h>
#include <Collider.h>
#include "Octree.h"

bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins);

Collider::Collider(Octree* tree) : tree(tree)
{
}


Collider::~Collider()
{
}

float Collider::rayIntersection(float3 rayOrigin, float3 rayVec) {
	std::vector<Triangle*> geometry;
	float3 rayVecInv = make_vector(1.0f / rayVec.x, 1.0f / rayVec.y, 1.0f / rayVec.z);
	if(rayVec.x < 0.001f) rayVecInv.x = 0.0f;
	if(rayVec.y < 0.001f) rayVecInv.y = 0.0f;
	if(rayVec.z < 0.001f) rayVecInv.z = 0.0f;


	tree->getGeometry(rayOrigin, rayVecInv, &geometry);

	float minIns = NULL;
	for (int i = 0; i < geometry.size(); i++) {
		Triangle t = *geometry[i];
		float ins;

		if (rayTriangle(rayOrigin, rayVec, t.p1, t.p2, t.p3, &ins)) {
			if ((minIns) >(ins) || minIns == NULL) {
				minIns = ins;
			}
		}
	}

	if (minIns == NULL || geometry.size() == 0) {
		return 0;
	}

	return -minIns;
}


void Collider::insertAll() {
	tree->insertAll(ts);
}


void Collider::addGameObject(GameObject*gameObject) {
	std::vector<Triangle*> gobTs = gameObject->getTriangles();
	ts.insert(ts.end(), gobTs.begin(), gobTs.end());
}


bool rayTriangle(float3 r_o, float3 r_d, float3 v1, float3 v2, float3 v3, float *ins)
{
	float3 e2 = v3 - v1;       // second edge
	float3 e1 = v2 - v1;       // first edge


	float3 r = cross(r_d, e2);  // (d X e2) is used two times in the formula
	// so we store it in an appropriate vector
	float3 s = r_o - v1;       // translated ray origin
	float a = dot(e1, r);    // a=(d X e2)*e1
	float f = 1.0f / a;           // slow division*
	float3 q = cross(s, e1);
	float u = dot(s, r);
	bool frontfacing = true;
	float eps = 0.0001f;
	if (a > eps)            // eps is the machine fpu epsilon (precision), 
		// or a very small number :)
	{ // Front facing triangle...
		if ((u<0) || (u>a)) return false;
		float v = dot(r_d, q);
		if ((v<0) || (u + v>a)) return false;
	}
	else if (a<-eps)
	{ // Back facing triangle...
		frontfacing = false;
		if ((u > 0) || (u < a)) return false;
		float v = dot(r_d, q);
		if ((v>0) || (u + v<a)) return false;
	}
	else return false; // Ray parallel to triangle plane
	float t = f*dot(e2, q);
	float v = dot(r_d, q);
	u = u*f;
	v = v*f;

	*ins = t;

	return true;
}

float getPointDistanceToPlane(float3 point, float3 normalVector, float d) {
    return normalVector.x * point.x + normalVector.y * point.y + normalVector.z * point.z + d;
}


bool TriangleTriangleIntersection(Triangle* t1, Triangle* t2) {

    //T1
    float3 normalVectorT1 = cross(t1->p2 - t1->p1, t1->p3 - t1->p1);
    float d1 = normalVectorT1.x * t1->p1.x + normalVectorT1.y * t1->p1.y + normalVectorT1.z * t1->p1.z;

    float t2DistToT1V1 = getPointDistanceToPlane(t1->p1, normalVectorT1, d1);
    float t2DistToT1V2 = getPointDistanceToPlane(t1->p2, normalVectorT1, d1);
    float t2DistToT1V3 = getPointDistanceToPlane(t1->p3, normalVectorT1, d1);

    if(t2DistToT1V1 != 0 && t2DistToT1V2 != 0 && t2DistToT1V3 != 0){
        if(t2DistToT1V1 > 0 && t2DistToT1V2 > 0 && t2DistToT1V3 > 0) { return false;}
        if(t2DistToT1V1 < 0 && t2DistToT1V2 < 0 && t2DistToT1V3 < 0) { return false;}
    }

    //T2
    float3 normalVectorT2 = cross(t2->p2 - t2->p1, t2->p3 - t2->p1);
    float d2 = normalVectorT2.x * t2->p1.x + normalVectorT2.y * t2->p1.y + normalVectorT2.z * t2->p1.z;

    float t1DistToT1V1 = getPointDistanceToPlane(t2->p1, normalVectorT2, d2);
    float t1DistToT1V2 = getPointDistanceToPlane(t2->p2, normalVectorT2, d2);
    float t1DistToT1V3 = getPointDistanceToPlane(t2->p3, normalVectorT2, d2);

    if(t1DistToT1V1 != 0 && t1DistToT1V2 != 0 && t1DistToT1V3 != 0){
        if(t1DistToT1V1 > 0 && t1DistToT1V2 > 0 && t1DistToT1V3 > 0) { return false;}
        if(t1DistToT1V1 < 0 && t1DistToT1V2 < 0 && t1DistToT1V3 < 0) { return false;}
    }


    return true;

}

































