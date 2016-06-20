//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H
#define SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H

#include "Utils.h"

/**
 * The exact phase collider is responsible for performing
 * exact collision tests.
 */
class ExactPhaseCollider {
public:
    /**
     * Takes a list of possibly colliding pairs and performs exact collision
     * tests between them.
     *
     */
    virtual void computeExactCollision(CollisionPairList possibleCollision) = 0;

};

#endif //SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H
