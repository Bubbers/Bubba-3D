//
// Created by johan on 2016-04-13.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
#define SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H

#include "Collider.h"

/**
 * Class responsible for creating colliders
 */
class ColliderFactory {
public:
    /**
     * Retuns a two phase collider. The first phase will be a fast, inaccurate test and
     * the second phase will be a exact test.
     */
    static Collider* getTwoPhaseCollider();
};

#endif //SUPER_BUBBA_AWESOME_SPACE_COLLIDERFACTORY_H
