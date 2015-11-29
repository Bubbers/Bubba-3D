//
// Created by johan on 2015-11-29.
//

#ifndef BUBBA_3D_CAR_H
#define BUBBA_3D_CAR_H

#include "float3.h"

using namespace chag;

struct Car{
    float3 frontDir;
    float3 upDir;
    float3 location;
    float3 wheel1, wheel2, wheel3,  wheel4;

    float rotationSpeed;
    float moveSpeed;
    float angley, anglez, anglex;
    float lengthx, lengthz;

    Car() {
        frontDir = make_vector(0.0f, 0.0f, 1.0f);
        upDir = make_vector(0.0f, 1.0f, 0.0f);
        location = make_vector(0.0f, 10.0f, 0.0f);
        wheel1 = make_vector( 1.2f, 0.0f,  1.5f);
        wheel2 = make_vector( 1.2f, 0.0f, -1.5f);
        wheel3 = make_vector(-0.8f, 0.0f,  1.5f);
        wheel4 = make_vector(-0.8f, 0.0f, -1.5f);

        rotationSpeed = (float) (2 * M_PI / 180);
        moveSpeed = 0.5;
        angley = 0; anglez = 0; anglex = 0;
        lengthx = 2; lengthz = 3;
    }
};


#endif //BUBBA_3D_CAR_H
