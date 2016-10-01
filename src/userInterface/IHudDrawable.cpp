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
// Created by simon on 2016-03-12.
//

#include <linmath/float3.h>
#include <linmath/SmallVector2.h>
#include "IHudDrawable.h"

void IHudDrawable::setRotation(float rotation) {
    this->rotation = rotation;
}

void IHudDrawable::setRelativePosition(chag::float3 position) {
    this->relativePosition = position;
}

void IHudDrawable::setCenterOffset(chag::float3 offset) {
    center = offset;
}

void IHudDrawable::setScale(chag::float2 scale) {
    this->scale = chag::make_vector(scale.x, scale.y, 1.0f);
}
