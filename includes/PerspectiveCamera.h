#ifndef __PESRPECTIVECAMERA_H__
#define __PESRPECTIVECAMERA_H__

#include "Camera.h"

class PerspectiveCamera : public Camera{ 
public:
	PerspectiveCamera(float3 position, float3 lookAt, float3 up, float fov, float ratio, float nearPlane, float farPlane);
	~PerspectiveCamera();

	float4x4 getViewMatrix();
	float4x4 getProjectionMatrix();

	void setPosition(float3 position);
	void setLookAt(float3 lookAt);
	void setUpVector(float3 up);
private:
	float4x4 lookAt(const float3 &eye, const float3 &center, const float3 &up);
	float4x4 perspectiveMatrix(float fov, float aspectRatio, float n, float f);
};

#endif //__PESRPECTIVECAMERA_H__