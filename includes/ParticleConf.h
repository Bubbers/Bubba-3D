#ifndef BUBBA_3D_PARTICLECONF_H
#define BUBBA_3D_PARTICLECONF_H

#include <GL/glew.h>
#include "float3.h"

using namespace chag;
class ParticleConf {


public:
    ParticleConf() { }
    virtual ~ParticleConf() { }

    virtual float3 calcPosition(float3 genPos) = 0;
    virtual float3 getVelocity() = 0;
    virtual float3 accelerate(float3 velocity) = 0;
    virtual float getLife() = 0;
    virtual float3 getScale() = 0;
    virtual bool loop(float dt) = 0;

    GLuint blendFunc = GL_ONE;

    void setLooping(bool value){
        looping = value;
    }


protected:
    bool looping = true;

};



#endif //BUBBA_3D_PARTICLECONF_H
