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
#ifndef __Triangle_h__
#define __Triangle_h__

#include <linmath/float3.h>

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
