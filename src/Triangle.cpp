#include "Triangle.h"
#include <math.h>

using namespace chag;

Triangle::Triangle(float3 p1, float3 p2, float3 p3) 
	:p1(p1), p2(p2), p3(p3)
{
	box = calculateBoundingBox();
}

Triangle::~Triangle(void) {

}

BoundingBox* Triangle::getBoundingBox(){
	return &box;
}

BoundingBox Triangle::calculateBoundingBox(){
	float maxX = fmax(fmax(p1.x, p2.x), p3.x);
	float maxY = fmax(fmax(p1.y, p2.y), p3.y);
	float maxZ = fmax(fmax(p1.z, p2.z), p3.z);

	float minX = fmin(fmin(p1.x, p2.x), p3.x);
	float minY = fmin(fmin(p1.y, p2.y), p3.y);
	float minZ = fmin(fmin(p1.z, p2.z), p3.z);

	BoundingBox b;

	b.points[0] = make_vector(maxX,  maxY, maxZ);
	b.points[1] = make_vector(maxX,  minY, maxZ);
	b.points[2] = make_vector(minX, maxY, maxZ);
	b.points[3] = make_vector(minX, minY, maxZ);

	b.points[4] = make_vector(maxX, maxY, minZ);
	b.points[5] = make_vector(maxX, minY, minZ);
	b.points[6] = make_vector(minX, maxY, minZ);
	b.points[7] = make_vector(minX, minY, minZ);

	return b;
}