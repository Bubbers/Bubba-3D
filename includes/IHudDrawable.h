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
#include <linmath/float4x4.h>
#include <memory>

class ShaderProgram;

/**
 * An interface for low-level objects used by HudRenderer to render the HUD.
 * Mainly produced by different implementations of Layout
 */
class IHudDrawable {
public:
    virtual void render(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix) = 0;

    /**
     * Sets a position relative to the original position calculated
     * by Layout implementations.
     */
    virtual void setRelativePosition(chag::float3 position);
    /**
     * Sets the rotation center offset. The original center is the actual center
     * of the drawn object.
     */
    virtual void setCenterOffset(chag::float3 offset);
    virtual void setRotation(float rotation);
    virtual void setScale(chag::float2 scale);
    chag::float3 relativePosition = chag::make_vector(0.0f, 0.0f, 0.0f);

protected:
    chag::float3 center = chag::make_vector(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;
    chag::float3 originalPosition = chag::make_vector(0.0f, 0.0f, 0.0f);
    chag::float3 scale = chag::make_vector(1.0f,1.0f,1.0f);
};
