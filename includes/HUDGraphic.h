//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H
#define SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H

#include <Dimension.h>

using namespace chag;

class HUDGraphic {

public:

    HUDGraphic(Texture* texture);
    HUDGraphic(Texture* texture, Dimension offsetX, Dimension offsetY);

    float3 getCenterOffset(float width, float height);
    Texture* getTexture();

protected:

    Dimension offsetX, offsetY;
    Texture* texture;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_HUDGRAPHIC_H
