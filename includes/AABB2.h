#ifndef __AABB_H__
#define __AABB_H__

#include <glutil/glutil.h>
#include <float.h>

struct AABB {
	chag::float3 maxV;
	chag::float3 minV;

	AABB() {
		maxV = chag::make_vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		minV = chag::make_vector(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	float getSize() {
		return sqrtf(pow(maxV.x - minV.x,2) + pow(maxV.y - minV.y,2) + pow(maxV.z - minV.z,2));
	}
};

#endif
