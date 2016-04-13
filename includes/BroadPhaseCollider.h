//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BROADPHASE_H
#define BUBBA_3D_BROADPHASE_H

#include <vector>
#include "Utils.h"
#include "Scene.h"

class GameObject;

class BroadPhaseCollider {
public:
    virtual CollisionPairList computeCollisionPairs(Scene *scene) = 0;


};
#endif //BUBBA_3D_BROADPHASE_H
