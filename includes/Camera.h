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

#include <linmath/float4x4.h>
#include <linmath/float3.h>
#include "IComponent.h"

/**
 * Interface for manipulating a camera
 */
class Camera : public IComponent{
public:
    Camera(chag::float3 position, chag::float3 lookAt,
           chag::float3 up, float fov, float ratio,
           float nearPlane, float farPlane):
    	 	   m_vPosition(position), m_vLookAt(lookAt), m_vUp(up), m_fFov(fov),
    		   m_fRatio(ratio), m_fNearPlane(nearPlane), m_fFarPlane(farPlane)
    {
    }

    ~Camera() = default;

    virtual void update(float dt) = 0;
    virtual chag::float4x4 getViewMatrix() = 0;
    virtual chag::float4x4 getProjectionMatrix() = 0;

    virtual void setPosition(chag::float3 position);
    virtual void setLookAt(chag::float3 lookAt);
    virtual void setUpVector(chag::float3 up);
    chag::float3 getPosition() { return m_vPosition; };
    chag::float3 getLookAt() { return m_vLookAt; };
    chag::float3 getUp() { return m_vUp; };
	
protected:
    chag::float3 m_vPosition;
    chag::float3 m_vLookAt;
    chag::float3 m_vUp;
    float m_fFov = 60.0f;
    float m_fRatio = 1.0f;
    float m_fNearPlane = 0.1f;
    float m_fFarPlane = 100.0f;
};
