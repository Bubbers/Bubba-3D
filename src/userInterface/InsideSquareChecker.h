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
