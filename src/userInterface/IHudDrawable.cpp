//
// Created by simon on 2016-03-12.
//

#include <linmath/float3.h>
#include "IHudDrawable.h"

void IHudDrawable::setRotation(float rotation) {
    this->rotation = rotation;
}

void IHudDrawable::setRelativePosition(float3 position) {
    this->relativePosition = position;
}

void IHudDrawable::setCenterOffset(float3 offset) {
    center = offset;
}