#include "particle/Particle.h"
#include "ParticleConf.h"

Particle::Particle(ParticleConf *conf, chag::float4x4 modelMatrix) {
    reset(conf, modelMatrix);
};

void Particle::reset(ParticleConf *conf, chag::float4x4 modelMatrix) {
    position = conf->initialPosition();
    chag::float4 vec = make_vector(position.x, position.y, position.z, 1.0f);
    chag::float4 mat = modelMatrix * vec;
    position.x = mat.x;
    position.y = mat.y;
    position.z = mat.z;
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

chag::float3 Particle::getPosition() {
    return position;
}
