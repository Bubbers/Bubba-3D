#include "Camera.h"

void Camera::setPosition(float3 position) {
	this->m_vPosition = position;
}

void Camera::setLookAt(float3 lookAt){
	this->m_vLookAt = lookAt;
}

void Camera::setUpVector(float3 up){
	this->m_vUp = up;
}