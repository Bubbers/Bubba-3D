#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <float4x4.h>
#include <float3.h>
#include <glutil/glutil.h>

using namespace chag;

class Camera {
public:
	Camera(float3 position, float3 lookAt, float3 up, float fov, float ratio, float nearPlane, float farPlane);
	~Camera();

	virtual float4x4 getViewMatrix();
	virtual float4x4 getProjectionMatrix();

	virtual void setPosition(float3 position);
	virtual void setLookAt(float3 lookAt);
	virtual void setUpVector(float3 up);
	float3 getPosition() { return m_vPosition; };
	
protected:
	float3 m_vPosition;
	float3 m_vLookAt;
	float3 m_vUp;
	float m_fFov;
	float m_fNearPlane;
	float m_fFarPlane;
	float m_fRatio;
};

#endif // ! __CAMERA_H__
