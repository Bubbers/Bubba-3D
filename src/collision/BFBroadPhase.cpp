//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include <Collider.h>
#include <timer.h>
#include <Logger.h>
#include <sstream>
#include <SFML/System/Clock.hpp>
#include "BFBroadPhase.h"

BFBroadPhase::BFBroadPhase() {

}

void BFBroadPhase::addGameObject(GameObject *gameObject){
    GameObjectList.push_back(gameObject);
}

void BFBroadPhase::removeDirty(){
	for (auto it = GameObjectList.begin(); it != GameObjectList.end();) {
		if ((*it)->isDirty()) {
			it = GameObjectList.erase(it);
		}
		else {
			it++;
		}
	}
}

void BFBroadPhase::updateCollision() {


    removeDirty();

    utils::Timer timer;
    timer.start();
    CollisionPairList possibleCollision = computeCollisionPairs();
    computeExactCollision(possibleCollision);

    timer.stop();
    stringstream timeMessage;
    timeMessage << "Tested " << possibleCollision.size() <<" possible collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
}


CollisionPairList BFBroadPhase::computeCollisionPairs() {
    CollisionPairList collisionPairs;
    int timeSpentOnCapsel = 0, timeSpentOnAABB = 0;

    for (auto i = GameObjectList.begin(); i != GameObjectList.end(); i++) {
        for (auto j = i + 1; j != GameObjectList.end(); j++) {
            GameObject* gameObject1 = *i;
            GameObject* gameObject2 = *j;

            if(isPossiblyColliding(gameObject1,gameObject2, &timeSpentOnCapsel,&timeSpentOnAABB)) {
                collisionPairs.push_back(std::pair<GameObject *, GameObject *>(gameObject1, gameObject2));
            }

        }
    }
    Logger::logDebug("Time spent on capsel: " + to_string(timeSpentOnCapsel) + ", on aabb: " + to_string(timeSpentOnAABB));
    return collisionPairs;
}


bool BFBroadPhase::isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2, int* spentOnCapsel, int* spentOnAABB) {
    if(!gameObject1->isDynamicObject() && !gameObject2->isDynamicObject()
       || gameObject1->isDirty() || gameObject2->isDirty() || gameObject1 == gameObject2) {
        return false;
    }
    sf::Clock clock = sf::Clock();
    bool capselInt = capselIntersection(gameObject1->getLocation(),gameObject1->getRadius(),
                                                 gameObject2->getLocation(),gameObject2->getRadius());
    *spentOnCapsel += clock.restart().asMicroseconds();
    if(capselInt) {

        AABB aabb1 = gameObject1->getAABB();
        AABB aabb2 = gameObject2->getAABB();

        bool aabb = AabbAabbintersection(&aabb1, &aabb2);
        *spentOnAABB += clock.getElapsedTime().asMicroseconds();
        return aabb;

    }

    return false;
}

void BFBroadPhase::computeExactCollision(CollisionPairList possibleCollision) {
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

void BFBroadPhase::triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType) {
    object1->callEvent(eventType, object2->getType());
    object2->callEvent(eventType, object1->getType());
}

void BFBroadPhase::rememberCollidingPair(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    collidingList.push_back(std::pair<int, int> (id1, id2));
}

void BFBroadPhase::forgetCollidingPair(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    collidingList.erase(std::remove(collidingList.begin(), collidingList.end(),
                                    std::pair<int, int>(id1, id2)));
}

bool BFBroadPhase::wasCollidingPreviously(GameObject* object1, GameObject* object2) {
    int id1 = getMinId(object1,object2);
    int id2 = getMaxId(object1,object2);

    return std::find(collidingList.begin(), collidingList.end(), std::pair<int, int>(id1, id2)) != collidingList.end();
}

int BFBroadPhase::getMinId(GameObject* object1, GameObject* object2) {
    return object1->getId() < object2->getId() ? object1->getId() : object2->getId();
}

int BFBroadPhase::getMaxId(GameObject* object1, GameObject* object2) {
    return object1->getId() > object2->getId() ? object1->getId() : object2->getId();
}

