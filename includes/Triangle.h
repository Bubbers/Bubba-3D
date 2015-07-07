#ifndef __Triangle_h__
#define __Triangle_h__

#include <float3.h>

using namespace chag;

struct BoundingBox {
	float3 points[8];
};

class Triangle {
public:
	Triangle(float3 p1, float3 p2, float3 p3);
	~Triangle(void);

	BoundingBox *getBoundingBox();

	float3 p1, p2, p3;
private:
	BoundingBox box;

	BoundingBox calculateBoundingBox();
};



#endif
