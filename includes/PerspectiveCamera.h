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

#include "Camera.h"

/**
 * \brief Implementation of the Camera interface.
 *
 * Provides a perspective projection.
 */
class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(chag::float3 position, chag::float3 lookAt, chag::float3 up,
                      float fov, float ratio, float nearPlane, float farPlane);
    ~PerspectiveCamera();

    chag::float4x4 getViewMatrix();
    chag::float4x4 getProjectionMatrix();

    void update(float dt);
    void setPosition(chag::float3 position);
    void setLookAt(chag::float3 lookAt);
    void setUpVector(chag::float3 up);
private:
    chag::float4x4 lookAt(const chag::float3 &eye, const chag::float3 &center, const chag::float3 &up);
    chag::float4x4 perspectiveMatrix(float fov, float aspectRatio, float n, float f);
};
