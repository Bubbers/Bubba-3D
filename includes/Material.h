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
#pragma once

#include "linmath/float3.h"
#include "Texture.h"
#include <memory>

struct Material {
    chag::float3 diffuseColor;
    chag::float3 ambientColor;
    chag::float3 specularColor;
    chag::float3 emissiveColor;
    float specularExponent;
    std::shared_ptr<Texture> diffuseTexture = NULL;
    std::shared_ptr<Texture> bumpMapTexture = NULL;
    std::shared_ptr<Texture> emissiveTexture = NULL;

    Material() {}

    Material(const chag::float3 &diffuseColor, const chag::float3 &ambientColor, const chag::float3 &specularColor,
             const chag::float3 &emissiveColor, float specularExponent, const std::shared_ptr<Texture> &diffuseTexture)
            : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(specularColor),
              emissiveColor(emissiveColor), specularExponent(specularExponent), diffuseTexture(diffuseTexture) {}

    Material(const chag::float3 &diffuseColor, const chag::float3 &ambientColor, const chag::float3 &specularColor,
             const chag::float3 &emissiveColor, float specularExponent, const std::shared_ptr<Texture> &diffuseTexture,
             const std::shared_ptr<Texture> &bumpMapTexture)
             : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(specularColor), emissiveColor(emissiveColor),
             specularExponent(specularExponent), diffuseTexture(diffuseTexture), bumpMapTexture(bumpMapTexture) {}

    Material(const chag::float3 &diffuseColor, const chag::float3 &ambientColor, const chag::float3 &specularColor,
             const chag::float3 &emissiveColor, float specularExponent, const std::shared_ptr<Texture> &diffuseTexture,
             const std::shared_ptr<Texture> &bumpMapTexture, const std::shared_ptr<Texture> &emissiveTexture)
            : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(specularColor),
              emissiveColor(emissiveColor), specularExponent(specularExponent), diffuseTexture(diffuseTexture),
              bumpMapTexture(bumpMapTexture), emissiveTexture(emissiveTexture) {}
};
