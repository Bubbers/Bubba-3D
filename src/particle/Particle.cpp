#include "Particle.h"
#include "ParticleConf.h"

/**
 *
 */
Particle::Particle(float3* pos, ParticleConf *conf) {
    generatorPos = pos;
    reset(conf);
};

void Particle::reset(ParticleConf *conf) {
    position = conf->initialPosition(*generatorPos);
    velocity = conf->initialVelocity();
    life     = conf->calcLifetime();
}

void Particle::update(float deltaTime, float distanceToCam, ParticleConf *conf) {
    velocity    = conf->accelerate(velocity);
    position   += velocity * deltaTime / 1000;
    life       -= deltaTime + (distanceToCam * 2);
}

bool Particle::isAlive() {
    return life > 0.0f;
}

