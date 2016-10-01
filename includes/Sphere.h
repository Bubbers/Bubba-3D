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
#pragma once

class Sphere{

public:
    Sphere() = default;
    Sphere(chag::float3 position, float radius): pos(position), radius(radius) {
    }
   
    chag::float3 getPosition() {
        return pos;
    }

    float getRadius() {
        return radius;
    }

    void setPosition(chag::float3 position) {
        pos = position;
    }

    void setRadius(float radius) {
        this->radius = radius;
    }

private:
    chag::float3 pos;
    float radius;

};
