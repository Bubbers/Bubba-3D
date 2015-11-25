#ifndef BUBBA_3D_PARTICLECONF_H
#define BUBBA_3D_PARTICLECONF_H

#include "float3.h"

using namespace chag;
class ParticleConf {


public:
    ParticleConf(float3 *generatorPos) {this->generatorPos = generatorPos;}
    ~ParticleConf() { }

    virtual float3 getVelocity() = 0;
    virtual float3 getAcceleration() = 0;
    virtual float getLife() = 0;

private:
    float3 *generatorPos;
};


#endif //BUBBA_3D_PARTICLECONF_H
