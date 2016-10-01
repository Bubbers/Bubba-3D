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
#include "Utils.h"
#include "ShaderProgram.h"
#include "GameObject.h"

float degreeToRad(const float degree) {
    return (float) (degree * M_PI / 180);
}

float radToDegree(const float rad) {
    return (float) (rad * 180 / M_PI);
}

float getRand(const float min, const float max) {
    //srand(time(NULL));
    const float range = max - min;
    return (((float) rand() / (float) RAND_MAX) * range) + min;
}

template <typename T, unsigned S>
T getRandomElem(const T (&ts)[S])
{
    if (S > 0) {
        int ix = (int) getRand(0.0f, S - 0.1f);
        return ts[ix];
    } else {
        return NULL;
    }
}

void updateMinAndMax(const float x, const float y, const float z, chag::float3* minV, chag::float3* maxV) {
    if (x < minV->x) {
        minV->x = x;
    }

    if (y < minV->y) {
       minV->y = y;
    }

    if (z < minV->z) {
       minV->z = z;
    }

    if (x > maxV->x) {
       maxV->x = x;
    }

    if (y > maxV->y) {
       maxV->y = y;
    }

    if (z > maxV->z) {
       maxV->z = z;
    }
}

chag::float3 createRandomVector(const float minValue, const float maxValue){
    return chag::make_vector(getRand(minValue,maxValue), getRand(minValue,maxValue), getRand(minValue,maxValue));
}
