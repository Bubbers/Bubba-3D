//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BRUTEFORCEBROADPHASE_H
#define BUBBA_3D_BRUTEFORCEBROADPHASE_H

#include "BroadPhaseCollision.h"
#include "GameObject.h"


class BFBroadPhase : BroadPhaseCollision
{
public:
    BFBroadPhase();

    virtual CollisionPairList computeCollisionPairs(Scene* scene) override ;
    virtual void updateCollision(Scene* scene) override ;
private:
    bool isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2);
    void computeExactCollision(CollisionPairList possibleCollision);
    void triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType);

    void forgetCollidingPair(GameObject* object1, GameObject* object2);
    void rememberCollidingPair(GameObject* object1, GameObject* object2);
    bool wasCollidingPreviously(GameObject* object1, GameObject* object2);

    int getMaxId(GameObject* object1, GameObject* object2);
    int getMinId(GameObject* object1, GameObject* object2);

    std::vector<std::pair<int, int>> collidingList;
};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
