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

#include "InsideSquareChecker.h"

InsideSquareChecker::InsideSquareChecker(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

bool InsideSquareChecker::isInside(int x, int y) {
    return this->x <= x && this->y <= y && (this->x + width) > x && (this->y + height) > y;
}