//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
#define SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H

#include "Collider.h"

class ColliderFactory {
public:
    static Collider* getTwoPhaseCollider();
};

#endif //SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
