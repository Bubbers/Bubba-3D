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

CollisionPairList BFBroadPhase::computeCollisionPairs(Scene *scene) {
    std::vector<std::shared_ptr<GameObject>> sceneObjects = scene->getGameObjects();
    CollisionPairList collisionPairs;

    for (auto i = sceneObjects.begin(); i != sceneObjects.end(); i++) {
        for (auto j = i + 1; j != sceneObjects.end(); j++) {
            std::shared_ptr<GameObject> gameObject1 = *i;
            std::shared_ptr<GameObject> gameObject2 = *j;

            if(isPossiblyColliding(gameObject1,gameObject2)) {
                collisionPairs.push_back(
                    std::pair<std::shared_ptr<GameObject>,std::shared_ptr<GameObject>>
                        (gameObject1, gameObject2));
            }

        }
    }
    return collisionPairs;
}


bool BFBroadPhase::isPossiblyColliding(std::shared_ptr<GameObject> gameObject1,
                                       std::shared_ptr<GameObject> gameObject2)
{
    // No need to check if none of the objects are dynamic
    if (!gameObject1->isDynamicObject() && !gameObject2->isDynamicObject()) {
        return false;
    }

    // Check if the objects are able to collide
    if (!gameObject1->collidesWith(gameObject2->getIdentifier())
         && !gameObject2->collidesWith(gameObject1->getIdentifier())) {
        return false;
    }

    // If either object is dirty or they are the same object abort
    if (gameObject1->isDirty() || gameObject2->isDirty() || gameObject1 == gameObject2) {
        return false;
    }

    // Abort if sphere intersection does not match
    if (!sphereIntersection(gameObject1->getTransformedSphere(), gameObject2->getTransformedSphere())) {
        return false;
    }

    // Do AABB test and return the result
    AABB aabb1 = gameObject1->getTransformedAABB();
    AABB aabb2 = gameObject2->getTransformedAABB();

    return AabbAabbintersection(&aabb1, &aabb2);
}
