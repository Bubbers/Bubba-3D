#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <linmath/float4x4.h>
#include <linmath/float3.h>
#include "IComponent.h"

using namespace chag;

/**
 * Interface for manipulating a camera
 */
class Camera : public IComponent{
public:
	Camera(float3 position, float3 lookAt, float3 up, float fov, float ratio, float nearPlane, float farPlane)
			: m_vPosition(position), m_vLookAt(lookAt), m_vUp(up), m_fFov(fov),
			  m_fRatio(ratio), m_fNearPlane(nearPlane), m_fFarPlane(farPlane){}
	~Camera() {}

	virtual void update(float dt) = 0;
	virtual float4x4 getViewMatrix() = 0;
	virtual float4x4 getProjectionMatrix() = 0;

	virtual void setPosition(float3 position);
	virtual void setLookAt(float3 lookAt);
	virtual void setUpVector(float3 up);
	float3 getPosition() { return m_vPosition; };
	float3 getLookAt() { return m_vLookAt; };
	float3 getUp() { return m_vUp; };
	
protected:
	float3 m_vPosition;
	float3 m_vLookAt;
	float3 m_vUp;
	float m_fFov = 60.0f;
	float m_fNearPlane = 0.1f;
	float m_fFarPlane = 100.0f;
	float m_fRatio = 1.0f;
};

#endif // ! __CAMERA_H__
