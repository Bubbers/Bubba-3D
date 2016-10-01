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
#ifndef BUBBA_3D_MATERIAL_H
#define BUBBA_3D_MATERIAL_H

#include "linmath/float3.h"
#include "Texture.h"

struct Material {
    chag::float3 diffuseColor;
    chag::float3 ambientColor;
    chag::float3 specularColor;
    chag::float3 emissiveColor;
    float specularExponent;
    Texture *diffuseTexture = NULL;
    Texture *bumpMapTexture = NULL;
};

#endif //BUBBA_3D_MATERIAL_H
