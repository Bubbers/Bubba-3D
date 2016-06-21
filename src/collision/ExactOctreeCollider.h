//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H
#define SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H

#include "../../includes/ExactPhaseCollider.h"

class ExactOctreeCollider : public ExactPhaseCollider{
public:
    CollisionPairList computeExactCollision(CollisionPairList possibleCollision) override ;

private:

};


#endif //SUPER_BUBBA_AWESOME_SPACE_EXACTOCTREECOLLIDER_H
