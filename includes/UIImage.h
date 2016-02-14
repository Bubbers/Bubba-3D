//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_UIIMAGE_H
#define SUPER_BUBBA_AWESOME_SPACE_UIIMAGE_H

#include <float2.h>
#include "Texture.h"

using namespace chag;

class UIImage {
public:
    Texture getTexture();
    float2 getTopLeftPosition();
    float2 getBottomRightPosition();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_UIIMAGE_H
