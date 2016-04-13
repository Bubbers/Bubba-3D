//
// Created by johan on 2016-04-13.
//

#include <BFBroadPhase.h>
#include "../../includes/ColliderFactory.h"
#include "ExactOctreeCollider.h"
#include "TwoPhaseCollider.h"

Collider* ColliderFactory::getTwoPhaseCollider() {
    return new TwoPhaseCollider(new BFBroadPhase(), new ExactOctreeCollider());
}
