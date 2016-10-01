//
// Created by johan on 2016-10-01.
//

#ifndef BUBBAROGUEFORT_BONEINFO_H
#define BUBBAROGUEFORT_BONEINFO_H

#include "linmath/float4x4.h"

struct BoneInfo
    {
        chag::float4x4 boneOffset;
        chag::float4x4 finalTransformation;
    };


#endif //BUBBAROGUEFORT_BONEINFO_H
