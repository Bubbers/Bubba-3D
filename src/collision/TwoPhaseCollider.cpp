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

#include <cstddef>
#include <timer.h>
#include <sstream>
#include "GameObject.h"
#include "TwoPhaseCollider.h"
#include "Logger.h"

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
    std::stringstream timeMessage;
    timeMessage << "Tested " << possibleCollision.size()
                <<" possible collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
}

void TwoPhaseCollider::triggerObjectEvents(CollisionPairList exactCollisions) {
    for(CollisionPair pair : exactCollisions) {

        if (wasCollidingPreviously(sortPair(pair))) {
            triggerObjectEvent(pair, EventType::DuringCollision);
        } else {
            triggerObjectEvent(pair, EventType::BeforeCollision);
        }
    }

    callEventsForObjectsThatNoLongerCollides(exactCollisions);
}

void TwoPhaseCollider::callEventsForObjectsThatNoLongerCollides(CollisionPairList exactCollisions) {
    CollisionPairList newCollidingList;

    for(CollisionPair pair : exactCollisions) {
        CollisionPair sortedPair = sortPair(pair);

        if (std::find(collidingList.begin(), collidingList.end(), sortedPair) != collidingList.end()) {
            collidingList.erase(std::remove(collidingList.begin(), collidingList.end(), sortedPair));
        }

        newCollidingList.push_back(sortedPair);
    }


    for(CollisionPair pair : collidingList) {
        triggerObjectEvent(pair, EventType::AfterCollision);
    }

    collidingList = newCollidingList;
}

void TwoPhaseCollider::triggerObjectEvent(CollisionPair &pair, EventType eventType) {
    pair.second->callEvent(eventType, pair.first);
    pair.first->callEvent(eventType, pair.second);
}

bool TwoPhaseCollider::wasCollidingPreviously(CollisionPair sortedPair) {
    return std::find(collidingList.begin(), collidingList.end(), sortedPair) != collidingList.end();
}

CollisionPair TwoPhaseCollider::sortPair(CollisionPair pair) {
    if (pair.first->getId() < pair.second->getId()) {
        return pair;
    }
    return CollisionPair(pair.second, pair.first);
}
