//
// Created by johan on 2016-04-13.
//

#include <Collider.h>
#include "ExactOctreeCollider.h"

void ExactOctreeCollider::computeExactCollision(CollisionPairList possibleCollision) {
    for(auto i = possibleCollision.begin(); i != possibleCollision.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        float4x4 object1ModelMatrix = object1->getModelMatrix();
        float4x4 object2ModelMatrix = object2->getModelMatrix();

        Octree* object1Oct = object1->getOctree();
        Octree* object2Oct = object2->getOctree();

        bool wasColliding = wasCollidingPreviously(object1, object2);

        if(octreeOctreeIntersection(object1Oct,&object1ModelMatrix,object2Oct, &object2ModelMatrix)) {
            if(wasColliding) {
                triggerObjectEvent(object1, object2, EventType::DuringCollision);
            } else {
                triggerObjectEvent(object1, object2, EventType::BeforeCollision);
                rememberCollidingPair(object1, object2);
            }
        } else if (wasColliding) {
            triggerObjectEvent(object1,object2, EventType::AfterCollision);
            forgetCollidingPair(object1, object2);
        }
    }
}

void ExactOctreeCollider::triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType) {
    object1->callEvent(eventType, object2);
    object2->callEvent(eventType, object1);
}

void ExactOctreeCollider::rememberCollidingPair(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    collidingList.push_back(std::pair<int, int> (id1, id2));
}

void ExactOctreeCollider::forgetCollidingPair(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    collidingList.erase(std::remove(collidingList.begin(), collidingList.end(),
                                    std::pair<int, int>(id1, id2)));
}

bool ExactOctreeCollider::wasCollidingPreviously(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    return std::find(collidingList.begin(), collidingList.end(), std::pair<int, int>(id1, id2)) != collidingList.end();
}

int ExactOctreeCollider::getMinId(GameObject* object1, GameObject* object2) {
    return object1->getId() < object2->getId() ? object1->getId() : object2->getId();
}

int ExactOctreeCollider::getMaxId(GameObject* object1, GameObject* object2) {
    return object1->getId() > object2->getId() ? object1->getId() : object2->getId();
}

