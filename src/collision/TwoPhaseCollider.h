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

#ifndef SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H
#define SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H

#include <BroadPhaseCollider.h>
#include <ExactPhaseCollider.h>
#include "../../includes/Collider.h"

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
    void triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType);

    void forgetCollidingPair(GameObject* object1, GameObject* object2);
    void rememberCollidingPair(GameObject* object1, GameObject* object2);
    bool wasCollidingPreviously(GameObject* object1, GameObject* object2);

    GameObject* getMaxId(GameObject* object1, GameObject* object2);
    GameObject* getMinId(GameObject* object1, GameObject* object2);

    CollisionPairList collidingList;

    BroadPhaseCollider *broadPhaseCollider;
    ExactPhaseCollider *exactPhaseCollider;

    void triggerObjectEvents(CollisionPairList vector);

    void callEventsForObjectsThatNoLongerCollides(CollisionPairList exactCollisions);
};


#endif //SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H
