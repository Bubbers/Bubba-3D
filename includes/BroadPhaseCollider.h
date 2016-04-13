//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BROADPHASE_H
#define BUBBA_3D_BROADPHASE_H

#include <vector>
#include "Utils.h"
#include "Scene.h"

class GameObject;

/**
 * The broad phase collider performs fast and inaccurate (many false positives, no false negatives)
 * collision detections. A more exact collision test should be performed after.
 */
class BroadPhaseCollider {
public:
    /**
     * Returns a list of possibly colliding pairs
     */
    virtual CollisionPairList computeCollisionPairs(Scene *scene) = 0;


};
#endif //BUBBA_3D_BROADPHASE_H
