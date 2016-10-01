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
// Created by simon on 2015-12-20.
//

#include <MouseWarp.h>

MouseWarp::MouseWarp(){
    warped = false;
}

MouseWarp::MouseWarp(int x, int y) {
    this->x = x;
    this->y = y;
    warped = true;
}

MouseWarp MouseWarp::noWarp() { return MouseWarp(); }

MouseWarp MouseWarp::warp(int x, int y) { return MouseWarp(x,y); }

bool MouseWarp::isWarped() {return warped; }

int MouseWarp::getX(){ return x;}
int MouseWarp::getY(){ return y;}