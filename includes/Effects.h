/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#pragma once

#include "linmath/float3.h"

namespace FOG_EQ {
    enum fog_equation {
        LINEAR = 0,
        EXP = 1,
        EXP2 = 2,
        NONE = 3
    };
};


struct Fog {
    float fDensity;
    float fStart;
    float fEnd;
    chag::float3 vColor;
    FOG_EQ::fog_equation fEquation;

    Fog() {
        fDensity = 0.001f;
        fStart = 50.0f;
        fEnd = 900.0f;
        vColor = chag::make_vector(1.0f, 1.0f, 1.0f);
        fEquation = FOG_EQ::NONE;
    }
};

struct Blur {
    float cutOff;
    bool active;

    Blur() {
        cutOff = 1.0f;
        active = true;
    }
};

struct Bloom {
    bool active;

    Bloom(){
        active = true;
    }
};

struct Wind {
      chag::float3 force;

    Wind() {
        force = chag::make_vector(0.0f, 0.0f, 0.0f);
    }
};

struct Effects {
    Fog fog;
    Blur blur;
    Bloom bloom;
    Wind wind;
};
