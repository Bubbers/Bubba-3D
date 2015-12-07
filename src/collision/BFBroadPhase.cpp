//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include "BFBroadPhase.h"

BFBroadPhase::BFBroadPhase() {

}

void BFBroadPhase::add(GameObject* gameObject) {
    GameObjectList.push_back(gameObject);
}

void BFBroadPhase::update() {

}

CollisionPairList BFBroadPhase::computeCollisionPairs() {
    CollisionPairList collisionPairs;

    for (auto i = GameObjectList.begin(); i != GameObjectList.end(); ++i) {
        for (auto j = ++i; j != GameObjectList.end(); j++) {
            GameObject* gameObject1 = *i;
            GameObject* gameObject2 = *j;


            if(gameObject1 != gameObject2 && intersects(gameObject1->getAABB(), gameObject2->getAABB())) {
                collisionPairs.push_back(std::pair<GameObject*,GameObject*>(gameObject1, gameObject2));
            }
        }
    }
    return collisionPairs;
}


bool BFBroadPhase::intersects(AABB* aabb1, AABB* aabb2) {
    if(aabb1->maxV.x < aabb2->minV.x) {return false;}
    if(aabb1->maxV.y < aabb2->minV.y) {return false;}
    if(aabb1->maxV.z < aabb2->minV.z) {return false;}

    if(aabb2->maxV.x < aabb1->minV.x) {return false;}
    if(aabb2->maxV.y < aabb1->minV.y) {return false;}
    if(aabb2->maxV.z < aabb1->minV.z) {return false;}

    return true;
}