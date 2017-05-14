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
#include <ResourceManager.h>
#include "ParticleGenerator.h"
#include "ParticleRenderer.h"
#include "constants.h"
#include "linmath/float3x3.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleConf.h"
#include "GameObject.h"


ParticleGenerator::ParticleGenerator(int maxParticles,
                                     std::shared_ptr<ParticleRenderer> renderer,
                                     std::shared_ptr<ParticleConf> conf,
                                     std::shared_ptr<Camera> camera)
                                   : maxParticles(maxParticles),
                                     renderer(renderer),
                                     conf(conf),
                                     camera(camera)
{
}


ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::render() {
    if (owner.expired()) {
        // nothing to render
        return;
    }

    std::shared_ptr<GameObject> owner_ptr = owner.lock();
    chag::float3 pos = owner_ptr->getAbsoluteLocation();
    renderer->render(particles, pos, conf);
}

void ParticleGenerator::update(float dt) {
    if (owner.expired()) {
        // nothing to update
        return;
    }

    std::shared_ptr<GameObject> owner_ptr = owner.lock();
    if (particles.empty()) {
        for (int i = 0; i < maxParticles; i++) {
            std::unique_ptr<Particle> part(new Particle(*conf, owner_ptr->getModelMatrix()));
            particles.push_back(std::move(part));
        }
    }

    float distance = length(camera->getPosition() - owner_ptr->getAbsoluteLocation());

    for (std::unique_ptr<Particle> &particle : particles) {
        if (particle->isAlive()){
            particle->update(dt, distance, *conf);
        }
        else if(conf->loop(dt)){
            particle->reset(*conf, owner_ptr->getModelMatrix());
        }
    }
}
