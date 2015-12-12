#ifndef BUBBA_3D_FIREPARTICLE_H
#define BUBBA_3D_FIREPARTICLE_H

#include "ParticleConf.h"

class FireParticle : public ParticleConf {
public:
    FireParticle() {}
    ~FireParticle() {}

    float3 calcPosition(float3 genPos);
    float3 getVelocity();
    float3 accelerate(float3 velocity);
    float getLife();
};


#endif //BUBBA_3D_FIREPARTICLE_H
