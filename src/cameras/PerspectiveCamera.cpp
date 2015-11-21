#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float3 position, float3 lookAt, float3 up, int fov, float ratio, float nearPlane, float farPlane) 
	: Camera(position, lookAt, up, fov, ratio, nearPlane, farPlane)
{

}

PerspectiveCamera::~PerspectiveCamera() {
}

float4x4 PerspectiveCamera::getViewMatrix() {
	return lookAt(m_vPosition, m_vLookAt, m_vUp);
}

float4x4 PerspectiveCamera::getProjectionMatrix() {
	return perspectiveMatrix(m_fFov, m_fRatio, m_fNearPlane, m_fFarPlane);
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