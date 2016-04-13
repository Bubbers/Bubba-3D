//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
#define SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H

#include "BroadPhaseCollider.h"
#include "ExactPhaseCollider.h"

class ColliderFactory {
public:
    static BroadPhaseCollider* getBroadPhaseCollider();
    static ExactPhaseCollider* getExactPhaseCollider();
};

#endif //SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
