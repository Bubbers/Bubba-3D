#ifndef BUBBA_3D_PARTICLECONF_H
#define BUBBA_3D_PARTICLECONF_H

#include "float3.h"

using namespace chag;
class ParticleConf {


public:
    ParticleConf() { }
    virtual ~ParticleConf() { }

    virtual float3 getVelocity() = 0;
    virtual float3 accelerate(float3 velocity) = 0;
    virtual float getLife() = 0;

protected:
    float getRand(float min, float max) {
        int decimals = 100000;
        float range = max - min;
        return (((rand() % decimals) / ((float)decimals)) * range) + min;
    }
};



#endif //BUBBA_3D_PARTICLECONF_H
