#include "Camera.h"

class PerspectiveCamera : public Camera{ 
public:
	PerspectiveCamera(float3 position, float3 lookAt, float3 up, int fov, float ratio, float nearPlane, float farPlane);
	~PerspectiveCamera();

	float4x4 getViewMatrix();
	float4x4 getProjectionMatrix();

	void setPosition(float3 position);
	void setLookAt(float3 lookAt);
	void setUpVector(float3 up);
private:
};