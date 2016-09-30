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
