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
#include "InsideRoundedSquareChecker.h"
#include "GLSquare.h"
#include "HUDGraphic.h"

InsideRoundedSquareChecker::InsideRoundedSquareChecker(int x, int y, int width, int height,
                                                       int rctl, int rctr, int rcbr, int rcbl)
                                                     : InsideSquareChecker(x, y, width, height),
                                                       rctl(rctl), rctr(rctr),
                                                       rcbr(rcbr), rcbl(rcbl)
{
}

bool InsideRoundedSquareChecker::isInside(int x, int y) {
    if (!InsideSquareChecker::isInside(x,y)) {
        return false;
    }

    int corner;
    x -= this->x;
    y -= this->y;

    if (x > width / 2) { //right half
        x = width - x;
        if (y > height / 2) { // lower half
            y = height - y;
            corner = rcbr;

        } else {// upper half
            corner = rctr;
        }
    } else { // left half
        if (y > height/2) { // lower half
            y = height - y;
            corner = rcbl;
        } else {// upper half
            corner = rctl;
        }
    }
    if (x > corner || y > corner) {
        return true;
    }

    chag::SmallVector2<int> vecFromCenter = chag::make_vector(x,y) - chag::make_vector(corner,corner);
    float fromCenterOfRoundedCorner = length(vecFromCenter);
    return fromCenterOfRoundedCorner <= corner;
}
