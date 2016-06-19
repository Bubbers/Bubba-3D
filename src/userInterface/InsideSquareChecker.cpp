//
// Created by simon on 2016-06-19.
//

#include "InsideSquareChecker.h"

InsideSquareChecker::InsideSquareChecker(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

bool InsideSquareChecker::isInside(int x, int y) {
    return this->x <= x && this->y <= y && (this->x + width) > x && (this->y + height) > y;
}