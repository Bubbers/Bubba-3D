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

#include <linmath/float3.h>

struct Light
{
    chag::float3 ambientColor;
    chag::float3 diffuseColor;
    chag::float3 specularColor;

    Light() {
        ambientColor  = chag::make_vector(0.0f, 0.0f, 0.0f);
        diffuseColor  = chag::make_vector(0.0f, 0.0f, 0.0f);
        specularColor = chag::make_vector(0.0f, 0.0f, 0.0f);
    }
};

struct Attenuation {
    float constant = 1.5f;
    float linear = 0.0f;
    float exp = 0.0f;
};

struct DirectionalLight : public Light {
    chag::float3 direction;
};

struct PointLight : public Light {
    chag::float3 position;
    Attenuation attenuation;
};

struct SpotLight : public Light {
    chag::float3 direction;
    chag::float3 position;
    Attenuation attenuation;
    float cutOff; //cos(rads) of the wanted angle 
    float outerCutOff; //used for smooth edges
};
