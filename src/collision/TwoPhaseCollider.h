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
