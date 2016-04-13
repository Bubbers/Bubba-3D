//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H
#define SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H

#include "../../includes/ExactPhaseCollider.h"

class ExactOctreeCollider : public ExactPhaseCollider{
public:
    void computeExactCollision(CollisionPairList possibleCollision) override ;

private:
    void triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType);

    void forgetCollidingPair(GameObject* object1, GameObject* object2);
    void rememberCollidingPair(GameObject* object1, GameObject* object2);
    bool wasCollidingPreviously(GameObject* object1, GameObject* object2);

    int getMaxId(GameObject* object1, GameObject* object2);
    int getMinId(GameObject* object1, GameObject* object2);

    std::vector<std::pair<int, int>> collidingList;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H
