
#include <float4x4.h>
#include <float3.h>
#include <glutil.h>

using namespace chag;

class Camera {
public:
	Camera(float3 position, float3 lookAt, float3 up, int fov, float ratio, float nearPlane, float farPlane);
	~Camera();

	float4x4 getViewMatrix();
	float4x4 getProjectionMatrix();

	void setPosition(float3 position);
	void setLookAt(float3 lookAt);
	void setUpVector(float3 up);
	
private:
	float3 m_vPosition;
	float3 m_vLookAt;
	float3 m_vUp;
	int m_iFov;
	float m_fNearPlane;
	float m_fFarPlane;
	float m_fRatio;
};