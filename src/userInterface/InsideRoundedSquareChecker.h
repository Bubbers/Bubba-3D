//
// Created by simon on 2016-06-19.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_INSIDEGLSQUARECHECKER_H
#define SUPER_BUBBA_AWESOME_SPACE_INSIDEGLSQUARECHECKER_H

#include "InsideSquareChecker.h"

class GLSquare;

class InsideRoundedSquareChecker : public InsideSquareChecker {
public:
    virtual bool isInside(int x, int y);
    InsideRoundedSquareChecker(int x, int y, int width, int height, int rctl, int rctr, int rcbr, int rcbl);

protected:
    int rctl, rctr, rcbr, rcbl;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_INSIDEGLSQUARECHECKER_H
