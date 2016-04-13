//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BRUTEFORCEBROADPHASE_H
#define BUBBA_3D_BRUTEFORCEBROADPHASE_H

#include "BroadPhaseCollider.h"
#include "GameObject.h"


class BFBroadPhase : public BroadPhaseCollider
{
public:
    BFBroadPhase();

    virtual CollisionPairList computeCollisionPairs(Scene* scene) override ;
private:
    bool isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2);

};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
