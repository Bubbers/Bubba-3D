/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
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
