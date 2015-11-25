#include "Particle.h"

Particle::Particle(float3* pos) :
        position(make_vector(0.0f, 0.0f, 0.0f)),
        velocity(make_vector(0.0f, 0.1f, 0.0f)),
        color(make_vector(0.0f, 0.0f, 0.0f)), life(0.0f) {
    startPosition = pos;
};

void Particle::reset() {
    float x = PARTICLE_SPEED;
    float y = fabs(PARTICLE_SPEED) * 3;
    float z = PARTICLE_SPEED;
    acceleration = make_vector(0.9f, 1.0f, 0.9f);
    position = *startPosition;
    velocity = make_vector(x, y, z);
    life     = PARTICLE_LIFE;
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
