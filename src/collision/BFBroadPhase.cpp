//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include <Collider.h>
#include <timer.h>
#include <Logger.h>
#include <sstream>
#include "BFBroadPhase.h"

BFBroadPhase::BFBroadPhase() {

}

void BFBroadPhase::addGameObject(GameObject *gameObject){
    GameObjectList.push_back(gameObject);
}


void BFBroadPhase::updateCollision() {
    utils::Timer timer;
    timer.start();

    CollisionPairList possibleCollision = computeCollisionPairs();

    for(auto i = possibleCollision.begin(); i != possibleCollision.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        float4x4 *object1ModelMatrix = object1->getModelMatrix();
        float4x4 *object2ModelMatrix = object2->getModelMatrix();

        Octree* object1Oct = object1->getOctree();
        Octree* object2Oct = object2->getOctree();

        if(octreeOctreeIntersection(object1Oct,object1ModelMatrix,object2Oct, object2ModelMatrix)) {
            object1->callEvent(EventType::OnCollision);
            object2->callEvent(EventType::OnCollision);
        }
    }

    timer.stop();

    stringstream timeMessage;
    timeMessage << "Tested collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
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

