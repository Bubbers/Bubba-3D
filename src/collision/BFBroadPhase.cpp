//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include <Collider.h>
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

    for (auto i = GameObjectList.begin(); i != GameObjectList.end(); i++) {
        for (auto j = i + 1; j != GameObjectList.end(); j++) {
            GameObject* gameObject1 = *i;
            GameObject* gameObject2 = *j;

            if(!gameObject1->isDynamicObject() && !gameObject2->isDynamicObject()) {
                continue;
            }

            if(gameObject1 != gameObject2 && AabbAabbintersection(gameObject1->getAABB(), gameObject2->getAABB())) {
                collisionPairs.push_back(std::pair<GameObject*,GameObject*>(gameObject1, gameObject2));
            }
        }
    }
    return collisionPairs;
}


