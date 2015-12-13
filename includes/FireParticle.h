#ifndef BUBBA_3D_FIREPARTICLE_H
#define BUBBA_3D_FIREPARTICLE_H

#include "ParticleConf.h"

using namespace chag;

class FireParticle : public ParticleConf {
public:
    FireParticle() {}
    ~FireParticle() {}

    float3 calcPosition(float3 genPos);
    float3 getVelocity();
    float3 accelerate(float3 velocity);
    float getLife();
    float3 getScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;
};


#endif //BUBBA_3D_FIREPARTICLE_H
