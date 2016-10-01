/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
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
