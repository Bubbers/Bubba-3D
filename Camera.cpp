#include "Camera.h"


Camera::Camera(float3 position, float3 lookAt, float3 up, int fov, float ratio, float nearPlane, float farPlane)
	: m_vPosition(position), m_vLookAt(lookAt), m_vUp(up), m_iFov(fov), m_fRatio(ratio), m_fNearPlane(nearPlane), m_fFarPlane(farPlane){

}
Camera::~Camera() {

}

float4x4 Camera::getViewMatrix() {
	return lookAt(m_vPosition, m_vLookAt, m_vUp);
}

float4x4 Camera::getProjectionMatrix() {
	return perspectiveMatrix(m_iFov, m_fRatio, m_fNearPlane, m_fFarPlane);
}

void Camera::setPosition(float3 position) {
	this->m_vPosition = position;
}

void Camera::setLookAt(float3 lookAt){
	this->m_vLookAt = lookAt;
}

void Camera::setUpVector(float3 up){
	this->m_vUp = up;
}