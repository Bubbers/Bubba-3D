#pragma once


#include <linmath\float3.h>

using namespace chag;

struct Light
{
	float3 color;

	Light() {
		color = make_vector(0.0f, 0.0f, 0.0f);
	}
};

struct DirectionalLight : public Light {
	float3 position;
};