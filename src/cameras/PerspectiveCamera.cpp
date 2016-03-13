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