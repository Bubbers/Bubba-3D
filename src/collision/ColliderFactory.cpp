//
// Created by johan on 2016-04-13.
//

#include <BFBroadPhase.h>
#include "../../includes/ColliderFactory.h"
#include "ExactOctreeCollider.h"

BroadPhaseCollider* ColliderFactory::getBroadPhaseCollider() {
    return new BFBroadPhase();
}

ExactPhaseCollider* ColliderFactory::getExactPhaseCollider() {
    return new ExactOctreeCollider();
}
