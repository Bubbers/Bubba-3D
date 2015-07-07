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
};

#endif
