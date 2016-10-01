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

PerspectiveCamera::PerspectiveCamera(float3 position, float3 lookAt, float3 up, float fov, float ratio, float nearPlane, float farPlane) 
	: Camera(position, lookAt, up, fov, ratio, nearPlane, farPlane)
{

}

PerspectiveCamera::~PerspectiveCamera() {
}

float4x4 PerspectiveCamera::getViewMatrix() {
	return lookAt(m_vPosition, m_vLookAt, m_vUp);
}

void PerspectiveCamera::update(float dt) { }

float4x4 PerspectiveCamera::lookAt(const float3 &eye, const float3 &center, const float3 &up)
{
    float3 dir = chag::normalize(eye - center);
    float3 right = chag::normalize(cross(up, chag::normalize(dir)));
    float3 newup = chag::normalize(cross(dir, right));
    float3x3 R = make_matrix(right, newup, dir);
    float4x4 invrot = make_matrix(transpose(R), make_vector(0.0f,0.0f,0.0f));
    return invrot * make_translation(-eye);
}

float4x4 PerspectiveCamera::getProjectionMatrix() {
	return perspectiveMatrix(m_fFov, m_fRatio, m_fNearPlane, m_fFarPlane);
}

float4x4 PerspectiveCamera::perspectiveMatrix(float fov, float aspectRatio, float n, float f)
{
    return make_perspective(fov, aspectRatio, n, f);
}

void PerspectiveCamera::setPosition(float3 position) {
	this->m_vPosition = position;
}

void PerspectiveCamera::setLookAt(float3 lookAt){
	this->m_vLookAt = lookAt;
}

void PerspectiveCamera::setUpVector(float3 up){
	this->m_vUp = up;
}