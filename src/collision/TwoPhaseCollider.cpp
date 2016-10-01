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
//
// Created by johan on 2016-04-13.
//

#include <cstddef>
#include <timer.h>
#include <sstream>
#include "GameObject.h"
#include "TwoPhaseCollider.h"

TwoPhaseCollider::TwoPhaseCollider(BroadPhaseCollider *broadPhaseCollider, ExactPhaseCollider *exactPhaseCollider) {
    this->broadPhaseCollider = broadPhaseCollider;
    this->exactPhaseCollider = exactPhaseCollider;
}

void TwoPhaseCollider::updateCollision(Scene *scene) {
    utils::Timer timer;
    timer.start();
    CollisionPairList possibleCollision = broadPhaseCollider->computeCollisionPairs(scene);
    CollisionPairList exactCollisions = exactPhaseCollider->computeExactCollision(possibleCollision);

    triggerObjectEvents(exactCollisions);

    timer.stop();
    stringstream timeMessage;
    timeMessage << "Tested " << possibleCollision.size() <<" possible collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
}

void TwoPhaseCollider::triggerObjectEvents(CollisionPairList exactCollisions) {
    for(auto i = exactCollisions.begin(); i != exactCollisions.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        if(wasCollidingPreviously(object1, object2)) {
            triggerObjectEvent(object1, object2, EventType::DuringCollision);
        } else {
            triggerObjectEvent(object1, object2, EventType::BeforeCollision);
        }
    }

    callEventsForObjectsThatNoLongerCollides(exactCollisions);
}

void TwoPhaseCollider::callEventsForObjectsThatNoLongerCollides(CollisionPairList exactCollisions) {
    CollisionPairList newCollidingList;

    for(auto i = exactCollisions.begin(); i != exactCollisions.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        GameObject* id1 = getMinId(object1,object2);
        GameObject* id2 = getMaxId(object1,object2);

        CollisionPair sortedPair(id1, id2);
        if(std::find(collidingList.begin(), collidingList.end(), sortedPair) != collidingList.end()) {
            collidingList.erase(std::remove(collidingList.begin(), collidingList.end(), sortedPair));
        }
        newCollidingList.push_back(sortedPair);
    }


    for(auto i = collidingList.begin(); i != collidingList.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;
        triggerObjectEvent(object1, object2, EventType::AfterCollision);
    }

    collidingList = newCollidingList;
}


void TwoPhaseCollider::triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType) {
    object1->callEvent(eventType, object2);
    object2->callEvent(eventType, object1);
}

void TwoPhaseCollider::rememberCollidingPair(GameObject* object1, GameObject* object2) {
    GameObject* id1 = getMinId(object1,object2);
    GameObject* id2 = getMaxId(object1,object2);

    collidingList.push_back(CollisionPair(id1, id2));
}

void TwoPhaseCollider::forgetCollidingPair(GameObject* object1, GameObject* object2) {
    GameObject* id1 = getMinId(object1,object2);
    GameObject* id2 = getMaxId(object1,object2);

    collidingList.erase(std::remove(collidingList.begin(), collidingList.end(),
                                    CollisionPair(id1, id2)));
}

bool TwoPhaseCollider::wasCollidingPreviously(GameObject* object1, GameObject* object2) {
    GameObject* id1 = getMinId(object1,object2);
    GameObject* id2 = getMaxId(object1,object2);

    return std::find(collidingList.begin(), collidingList.end(), CollisionPair(id1, id2)) != collidingList.end();
}

GameObject* TwoPhaseCollider::getMinId(GameObject* object1, GameObject* object2) {
    return object1->getId() < object2->getId() ? object1 : object2;
}

GameObject* TwoPhaseCollider::getMaxId(GameObject* object1, GameObject* object2) {
    return object1->getId() > object2->getId() ? object1 : object2;
}







