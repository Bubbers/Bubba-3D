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
#include "PerspectiveCamera.h"
#include "linmath/float3x3.h"

PerspectiveCamera::PerspectiveCamera(chag::float3 position, chag::float3 lookAt, chag::float3 up,
                                     float fov, float ratio, float nearPlane, float farPlane) 
                                   : Camera(position, lookAt, up, fov, ratio, nearPlane, farPlane)
{

}

PerspectiveCamera::~PerspectiveCamera() {
}

chag::float4x4 PerspectiveCamera::getViewMatrix() {
    return lookAt(m_vPosition, m_vLookAt, m_vUp);
}

void PerspectiveCamera::update(float dt) { }

chag::float4x4 PerspectiveCamera::lookAt(const chag::float3 &eye,
                                         const chag::float3 &center,
                                         const chag::float3 &up)
{
    chag::float3 dir = chag::normalize(eye - center);
    chag::float3 right = chag::normalize(chag::cross(up, chag::normalize(dir)));
    chag::float3 newup = chag::normalize(chag::cross(dir, right));

    chag::float3x3 R = chag::make_matrix(right, newup, dir);
    chag::float4x4 invrot = chag::make_matrix(chag::transpose(R), chag::make_vector(0.0f,0.0f,0.0f));

    return invrot * chag::make_translation(-eye);
}

chag::float4x4 PerspectiveCamera::getProjectionMatrix() {
    return perspectiveMatrix(m_fFov, m_fRatio, m_fNearPlane, m_fFarPlane);
}

chag::float4x4 PerspectiveCamera::perspectiveMatrix(float fov, float aspectRatio, float n, float f) {
    return chag::make_perspective(fov, aspectRatio, n, f);
}

void PerspectiveCamera::setPosition(chag::float3 position) {
    this->m_vPosition = position;
}

void PerspectiveCamera::setLookAt(chag::float3 lookAt){
    this->m_vLookAt = lookAt;
}

void PerspectiveCamera::setUpVector(chag::float3 up){
    this->m_vUp = up;
}
