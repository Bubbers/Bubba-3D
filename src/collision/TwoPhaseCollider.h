//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H
#define SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H

#include <BroadPhaseCollider.h>
#include <ExactPhaseCollider.h>
#include "../../includes/Collider.h"

class TwoPhaseCollider : public Collider{
public:
    TwoPhaseCollider(BroadPhaseCollider *broadPhaseCollider, ExactPhaseCollider *exactPhaseCollider);
    void updateCollision(Scene *scene);

private:
    BroadPhaseCollider *broadPhaseCollider;
    ExactPhaseCollider *exactPhaseCollider;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_TWOPHASECOLLIDER_H
