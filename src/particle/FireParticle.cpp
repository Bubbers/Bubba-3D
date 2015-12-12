#include "FireParticle.h"

float3 FireParticle::calcPosition(float3 genPos) {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 0.3f);
    return make_vector(genPos.x + (float)cos(rand) * rand2, genPos.y, genPos.z + (float)sin(rand) * rand2);
}

float3 FireParticle::getVelocity() {
    return make_vector(getRand(-.5f, .5f), getRand(0.0f, 1.0f), getRand(-.5f, .5f));
}

float3 FireParticle::accelerate(float3 velocity) {
    return make_vector(.9f * velocity.x, 1.06f * velocity.y, .9f * velocity.z);
}

float FireParticle::getLife() {
    return getRand(0.0f, 2000.0f);
}