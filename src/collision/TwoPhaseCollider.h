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
#pragma once

#include <BroadPhaseCollider.h>
#include <ExactPhaseCollider.h>
#include "Collider.h"
#include "GameObject.h" // Required to get EventType

class GameObject;
class Scene;

/**
 * Class responsible for maintaining a two phase collider, ie
 * first a fast inaccurate test and then a more exact test on
 * all possibly colliding objects.
 */
class TwoPhaseCollider : public Collider{
public:
    TwoPhaseCollider(BroadPhaseCollider *broadPhaseCollider, ExactPhaseCollider *exactPhaseCollider);

    void updateCollision(Scene *scene) override;

private:
    void triggerObjectEvent(CollisionPair &pair, EventType eventType);

    bool wasCollidingPreviously(CollisionPair &pair);

    CollisionPairList collidingList;

    BroadPhaseCollider *broadPhaseCollider;
    ExactPhaseCollider *exactPhaseCollider;

    void sortPair(CollisionPair &pair);

    void triggerObjectEvents(CollisionPairList vector);

    void callEventsForObjectsThatNoLongerCollides(CollisionPairList exactCollisions);

    bool findPairInList(CollisionPair &pair, CollisionPairList &list);
};
