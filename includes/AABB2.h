#ifndef __AABB_H__
#define __AABB_H__

#include <glutil\glutil.h>

struct AABB {
	chag::float3 maxV = chag::make_vector(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	chag::float3 minV = chag::make_vector(FLT_MAX, FLT_MAX, FLT_MAX);
};

#endif