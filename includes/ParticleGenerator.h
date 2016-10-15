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
#pragma once

#include <memory>
#include <vector>

#include "IRenderComponent.h"


class Camera;
class Particle;
class ParticleConf;
class ParticleRenderer;


/**
 * \brief Component that generates, handles and renders particles.
 *
 * ParticleGenerator manages spawns and updates particles.
 *
 * All particles are spawned at a position relative to the GameObject
 * that the ParticleGenerator is attached to.
 *
 * Each particle is updated via a ParticleConf. The ParticleGenerator does
 * not interfere in the calculations but handles what ParticleConf to use.
 */
class ParticleGenerator : public IRenderComponent
{
public:

    /**
     * \brief Creates a ParticleGenerator that spawns particles with a given Texture
     *
     * @param maxParticles  The maximum amount of particles to spawn (less particles are spawned
     *                     when the camera is far away).
     * @param renderer     The ParticleRenderer that will be used to render particles.
     * @param conf         The ParticleConf that will be used while updating the particles.
     * @param camera       A reference to the active Camera in the scene. Used to compensate for
     *                     camera distance when updating Particles.
     */
    ParticleGenerator(int maxParticles,
                      std::shared_ptr<ParticleRenderer> renderer,
                      std::shared_ptr<ParticleConf> conf,
                      std::shared_ptr<Camera> camera);

    ~ParticleGenerator();

    /**
     * Update the position of all spawned Particles.
     * @param dt The time in seconds since last update call.
     */
    void update(float dt);

    /**
     * Render shadows from the spawned Particles.
     * @param shaderProgram The ShaderProgram used to render the shadows.
     */
    void renderShadow(ShaderProgram* shaderProgram) {};

    /**
     * Render all the spawned Particles.
     */
    void render();

private:

    int maxParticles = 0;

    std::vector<std::unique_ptr<Particle>> particles;

    std::shared_ptr<ParticleRenderer> renderer;
    std::shared_ptr<ParticleConf> conf;
    std::shared_ptr<Camera> camera;

};
