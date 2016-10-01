/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
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

	chag::float3 getCenterPosition() {
		return (maxV - minV) / 2;
	}
};

#endif
