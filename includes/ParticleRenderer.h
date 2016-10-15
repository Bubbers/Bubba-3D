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

#define LINEAR_SCALE_FACTOR 50.0f
#define LOD_FACTOR 25.0f
class Texture;
class Camera;
class ShaderProgram;
class ParticleConf;
class Particle;

/**
 * \bief Responsible for rendering particles
 *
 * ParticleRenderer renders Particles. Typically the particles are provided
 * by a ParticleGenerator.
 */
class ParticleRenderer
{
public:
    /**
     * \breif Creates a particle Renderer that renders particles
     *
     * @param texture The texture to use on the particles.
     * @param camera  A reference to the active Camera in the scene. Used to rotate particles
     *                towards the camera and compensate for camera distance.
     * @param shader  A reference to the ShaderProgram used.
     */
    ParticleRenderer(std::shared_ptr<Texture> texture,
                     std::shared_ptr<Camera> camera,
                     std::shared_ptr<ShaderProgram> shaderProgram=defaultShader());

    ~ParticleRenderer();

    /**
     * \brief render particles in the scene.
     *
     * @param particles The particles to render.
     */
    void render(std::vector<std::unique_ptr<Particle>> &particles,
                const chag::float3 &position,
                const std::shared_ptr<ParticleConf> &conf);

    static std::shared_ptr<ShaderProgram> defaultShader();

private:
    GLuint vaob;

    std::shared_ptr<Texture> texture;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ShaderProgram> shaderProgram;
};

