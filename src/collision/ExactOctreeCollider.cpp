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
#include <Collider.h>
#include "GameObject.h"
#include "ExactOctreeCollider.h"

CollisionPairList ExactOctreeCollider::computeExactCollision(CollisionPairList possibleCollision) {
    CollisionPairList exactCollisions;

    for(auto i = possibleCollision.begin(); i != possibleCollision.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        float4x4 object1ModelMatrix = object1->getModelMatrix();
        float4x4 object2ModelMatrix = object2->getModelMatrix();

        Octree* object1Oct = object1->getOctree();
        Octree* object2Oct = object2->getOctree();

        if(octreeOctreeIntersection(object1Oct,&object1ModelMatrix,object2Oct, &object2ModelMatrix)) {
            exactCollisions.push_back(CollisionPair(object1, object2));
        }
    }

    return exactCollisions;
}

