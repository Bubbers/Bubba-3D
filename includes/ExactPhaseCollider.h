//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H
#define SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H

#include "Utils.h"

class ExactPhaseCollider {
public:
    virtual void computeExactCollision(CollisionPairList possibleCollision) = 0;

};

#endif //SUPER_BUBBA_AWESOME_SPACE_EXACTPHASE_H
