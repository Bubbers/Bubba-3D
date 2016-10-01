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

#include "Scene.h"
#include "linmath/float4x4.h"

class Octree;

bool octreeOctreeIntersection(Octree *object1Octree,
                              chag::float4x4 *object1ModelMatrix,
                              Octree *object2Octree,
                              chag::float4x4 *object2ModelMatrix);

bool AabbAabbintersection(AABB *aabb1, AABB *aabb2);

bool sphereIntersection(Sphere sphere1,Sphere sphere2);

AABB multiplyAABBWithModelMatrix(AABB *aabb, chag::float4x4 modelMatrix);

bool triangleTriangleIntersection(Triangle *t1, Triangle *t2);

bool isTrianglesIntersecting(Octree *object1Octree,
                             chag::float4x4 *object1ModelMatrix,
                             Octree *object2Octree,
                             chag::float4x4 *object2ModelMatrix);


/**
 * Class responsible for dynamic collision tests in a scene.
 * Every colliding pair in the scene will at collisions have
 * a collision event called.
 */
class Collider
{
public:
    /**
     * Calls collision events on all colliding scene objects.
     */
    virtual void updateCollision(Scene *scene) = 0;

private:

};
