#include "Particle.h"

Particle::Particle(float3* pos, ParticleConf *conf) {
    generatorPos = pos;
    reset(conf);
};

void Particle::reset(ParticleConf *conf) {
    position = *generatorPos;
    velocity = conf->getVelocity();
    acceleration = conf->getAcceleration(velocity);
    life     = conf->getLife();
}

void Particle::update(float deltaTime, float distanceToCam) {
    velocity.x *= acceleration.x;
    velocity.y *= acceleration.y;
    velocity.z *= acceleration.z;
    position   += velocity * deltaTime / 1000;
    life       -= deltaTime + (distanceToCam * 2);
}

bool Particle::isAlive() {
    return life > 0.0f;
}
