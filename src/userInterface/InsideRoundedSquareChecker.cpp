//
// Created by simon on 2016-06-19.
//

#include "InsideRoundedSquareChecker.h"
#include "GLSquare.h"
#include "HUDGraphic.h"

InsideRoundedSquareChecker::InsideRoundedSquareChecker(int x, int y, int width, int height, int rctl, int rctr, int rcbr, int rcbl)
    : InsideSquareChecker(x, y, width, height), rctl(rctl), rctr(rctr), rcbr(rcbr), rcbl(rcbl) { }

bool InsideRoundedSquareChecker::isInside(int x, int y) {
    if(!InsideSquareChecker::isInside(x,y))
        return false;
    int corner;
    x = x-this->x;
    y = y-this->y;
    if(x > width/2){ //right half
        x = width - x;
        if(y > height/2) { // lower half
            y = height - y;
            corner = rcbr;
        }else // upper half
            corner = rctr;
    }else{ // left half
        if(y > height/2) { // lower half
            y = height - y;
            corner = rcbl;
        }else // upper half
            corner = rctl;
    }
    if(x > corner || y > corner){
        return true;
    }
    chag::SmallVector2<int> vecFromCenter = make_vector(x,y)-make_vector(corner,corner);
    float fromCenterOfRoundedCorner = length(vecFromCenter);
    return fromCenterOfRoundedCorner <= corner;

}