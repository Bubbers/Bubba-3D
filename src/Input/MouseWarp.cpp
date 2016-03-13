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