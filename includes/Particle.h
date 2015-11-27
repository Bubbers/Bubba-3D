#ifndef __PARTICLE__
#define __PARTICLE__

#include "float3.h"
#include "ParticleConf.h"

#define PARTICLE_SPEED ((rand() % 6000) - 3000.0f) / 5000.0f
#define PARTICLE_LIFE 1000.0f + (rand() % 1000)

using namespace chag;

class Particle {

public:
    Particle(float3 *pos, ParticleConf *conf);
    void reset(ParticleConf *conf);
    bool isAlive();
    void update(float deltaTime, float distanceToCam, ParticleConf *conf);
    float3 getPosition() { return position; }

private:
    float life; //in ms
    float3 position;
    float3 velocity;
    float3 color;
    float3* generatorPos;
};

#endif
