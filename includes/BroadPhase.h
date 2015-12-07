//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BROADPHASE_H
#define BUBBA_3D_BROADPHASE_H

#include <vector>
#include "GameObject.h"

typedef std::pair<GameObject*, GameObject*> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;

class BroadPhase {
public:
    virtual void add(GameObject* aabb) = 0;
    virtual void update() = 0;
    virtual CollisionPairList computeCollisionPairs() = 0;


};
#endif //BUBBA_3D_BROADPHASE_H
