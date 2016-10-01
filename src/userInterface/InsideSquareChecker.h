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
// Created by simon on 2016-06-19.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_INSIDESQUARECHECKER_H
#define SUPER_BUBBA_AWESOME_SPACE_INSIDESQUARECHECKER_H

#include "InsideChecker.h"

class InsideSquareChecker : public InsideChecker {
public:
    InsideSquareChecker(int x, int y, int width, int height);
    virtual bool isInside(int x, int y);

protected:
    int x,y,width,height;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_INSIDESQUARECHECKER_H
