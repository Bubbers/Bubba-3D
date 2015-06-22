#pragma once


#include <linmath\float3.h>

using namespace chag;

struct Light
{
	float3 ambientColor;
	float3 diffuseColor;
	float3 specularColor;

	Light() {
		ambientColor  = make_vector(0.0f, 0.0f, 0.0f);
		diffuseColor  = make_vector(0.0f, 0.0f, 0.0f);
		specularColor = make_vector(0.0f, 0.0f, 0.0f);
	}
};

struct DirectionalLight : public Light {
	float3 direction;
};

struct PointLight : public Light {
	float3 position;

	struct {
		float constant;
		float linear;
		float exp;
	} attenuation;
};

struct SpotLight : public Light {
	float3 direction;
	float3 position;
	float cutOff; //cos(rads) of the wanted angle 
	float outerCutOff; //used for smooth edges
};