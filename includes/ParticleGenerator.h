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

#include "linmath/float3.h"
#include "IRenderComponent.h"

#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f

class Camera;
class Texture;
class Particle;
class ParticleConf;


/**
 * \brief Component that generates, handles and renders particles.
 *
 * ParticleGenerator has three main responsabilities:
 *
 * 1. Generating particles
 * 2. Updating particles
 * 3. Rendering particles
 *
 * All particles are spawned at a position relative to the GameObject
 * that the ParticleGenerator is attached to.
 *
 * Each particle is updated via a ParticleConf. The ParticleGenerator does
 * not interfere in the calculations but handles what ParticleConf to use.
 *
 * If the camera is far away the ParticleGenerator adapts by not spawning
 * as many particles.
 */
class ParticleGenerator : public IRenderComponent
{
public:

    /**
     * \brief Creates a ParticleGenerator that spawns particles with a given Texture
     *
     * @param texture The texture that each particle will use while rendering.
     * @param amount  The maximum amount of particles to spawn (less particles are spawned
     *                when the camera is far away).
     * @param camera  A reference to the active Camera in the scene. Used to rotate particles
     *                towards the camera and compensate for camera distance.
     * @param conf    The ParticleConf that will be used while updating the particles.
     */
    ParticleGenerator(Texture *texture, int amount,
                      Camera *camera,
                      ParticleConf *conf);

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

    /**
     * Set how much the level of detail scales with distance to camera
     */
    void setScaleLod(bool value);

private:
    GLuint m_vaob;
    Texture *texture;

    int m_amount = 0;
    Camera *m_camera;

    std::vector<std::unique_ptr<Particle>> m_particles;

    std::unique_ptr<ParticleConf> conf;
    bool doScale = true;

    chag::float3x3 getModelMatrix3x3();
};
