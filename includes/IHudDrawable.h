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

#ifndef SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
#define SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H

#include <linmath/float3.h>
#include <linmath/float4x4.h>

using namespace chag;

class ShaderProgram;

/**
 * An interface for low-level objects used by HudRenderer to render the HUD.
 * Mainly produced by different implementations of Layout
 */
class IHudDrawable {
public:
    virtual void render(ShaderProgram* shaderProgram, float4x4* projectionMatrix) = 0;

    /**
     * Sets a position relative to the original position calculated
     * by Layout implementations.
     */
    virtual void setRelativePosition(float3 position);
    /**
     * Sets the rotation center offset. The original center is the actual center
     * of the drawn object.
     */
    virtual void setCenterOffset(float3 offset);
    virtual void setRotation(float rotation);
    virtual void setScale(float2 scale);
    float3 relativePosition = make_vector(0.0f, 0.0f, 0.0f);

protected:
    float3 center = make_vector(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;
    float3 originalPosition = make_vector(0.0f, 0.0f, 0.0f);
    float3 scale = make_vector(1.0f,1.0f,1.0f);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_IHUDDRAWABLE_H
