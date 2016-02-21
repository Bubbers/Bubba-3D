//
// Created by simon on 2016-02-21.
//

#include <float3.h>
#include <Texture.h>
#include <Dimension.h>
#include "HUDGraphic.h"


HUDGraphic::HUDGraphic(Texture* texture) : HUDGraphic(texture,Dimension(),Dimension()) {}

HUDGraphic::HUDGraphic(Texture *texture, Dimension offsetX, Dimension offsetY)
        : texture(texture), offsetX(offsetX), offsetY(offsetY){ }

float3 HUDGraphic::getCenterOffset(float width, float height) {
    return make_vector(offsetX.getSize(width),offsetY.getSize(height),0.0f);
}

Texture* HUDGraphic::getTexture() {
    return texture;
}