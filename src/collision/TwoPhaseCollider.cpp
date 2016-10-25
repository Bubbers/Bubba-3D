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
#include <memory>

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
    for(CollisionPair &pair : exactCollisions) {

        if (wasCollidingPreviously(pair)) {
            triggerObjectEvent(pair, EventType::DuringCollision);
        } else {
            triggerObjectEvent(pair, EventType::BeforeCollision);
        }
    }

    callEventsForObjectsThatNoLongerCollides(exactCollisions);
}

void TwoPhaseCollider::callEventsForObjectsThatNoLongerCollides(CollisionPairList exactCollisions) {
    CollisionPairList newCollidingList;

    for(CollisionPair &pair : exactCollisions) {
        sortPair(pair);

        if (findPairInList(pair, collidingList)) {
            collidingList.erase(std::remove(collidingList.begin(), collidingList.end(), pair));
        }

        newCollidingList.push_back(pair);
    }

    for(CollisionPair &pair : collidingList) {
        triggerObjectEvent(pair, EventType::AfterCollision);
    }

    collidingList = newCollidingList;
}

void TwoPhaseCollider::sortPair(CollisionPair &pair) {
    if (pair.first->getId() >= pair.second->getId()) {
        std::shared_ptr<GameObject> temp = pair.first;

        pair.first = pair.second;
        pair.second = temp;
    }
}

void TwoPhaseCollider::triggerObjectEvent(CollisionPair &pair, EventType eventType) {
    pair.second->callEvent(eventType, pair.first);
    pair.first->callEvent(eventType, pair.second);
}

bool TwoPhaseCollider::wasCollidingPreviously(CollisionPair &pair) {
    sortPair(pair);
    return findPairInList(pair, collidingList);
}

bool TwoPhaseCollider::findPairInList(CollisionPair &pair, CollisionPairList &list) {
    return std::find(list.begin(), list.end(), pair) != list.end();
}
