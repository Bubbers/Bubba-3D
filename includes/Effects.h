#ifndef __EFFECTS_H__
#define __EFFECTS_H__

#include "float3.h"

using namespace chag;
namespace FOG_EQ {
  enum fog_equation { LINEAR = 0, EXP = 1, EXP2 = 2, NONE = 3 };
};


struct Fog {
  float fDensity;
  float fStart;
  float fEnd;
  float3 vColor;
  FOG_EQ::fog_equation fEquation;

  Fog() {
    fDensity = 0.001f;
    fStart = 50.0f;
    fEnd = 900.0f;
    vColor = make_vector(1.0f, 1.0f, 1.0f);
    fEquation = FOG_EQ::NONE;
  }
};

struct Blur {
  float cutOff;
  bool active;

  Blur() {
    cutOff = 1.0f;
    active = false;
  }
};


struct Effects {
  Fog fog;
  Blur blur;
};

#endif //__EFFECTS_H__
