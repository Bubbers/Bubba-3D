//
// Created by johan on 2016-04-13.
//

#include <Collider.h>
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

