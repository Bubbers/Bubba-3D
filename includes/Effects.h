#ifndef __EFFECTS_H__
#define __EFFECTS_H__

#include "float3.h"

using namespace chag;
namespace FOG_EQ {
	enum fog_equation { LINEAR = 0, EXP = 1, EXP2 = 2, NONE = 3 };
};


struct Fog {
	float fDensity = 0.001f;
	float fStart = 50.0f;
	float fEnd = 5000.0f;
	float3 vColor = make_vector(1.0f, 1.0f, 1.0f);
	FOG_EQ::fog_equation fEquation = FOG_EQ::NONE;
};

struct Blur {
	float cutOff = 1.0f;
	bool active = false;
};


struct Effects {
	Fog fog;
	Blur blur;
};

#endif //__EFFECTS_H__
