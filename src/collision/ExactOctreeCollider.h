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
