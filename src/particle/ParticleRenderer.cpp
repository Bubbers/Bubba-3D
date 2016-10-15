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

#include "linmath/float3x3.h"

#include "ResourceManager.h"
#include "constants.h"

#include "Camera.h"
#include "ShaderProgram.h"
#include "Particle.h"
#include "ParticleRenderer.h"
#include "ParticleConf.h"


ParticleRenderer::ParticleRenderer(std::shared_ptr<Texture> texture,
                                   std::shared_ptr<Camera> camera,
                                   std::shared_ptr<ShaderProgram> shaderProgram)
                                 : texture(texture),
                                   camera(camera),
                                   shaderProgram(shaderProgram)
{
    GLfloat quad[] = { //POSITION3 TEXCOORD2
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    
    glGenVertexArrays(1, &vaob);
    glBindVertexArray(vaob);
    
    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    /* CLEANUP */
    glBindVertexArray(0);
    
}

ParticleRenderer::~ParticleRenderer()
{

}

std::shared_ptr<ShaderProgram> ParticleRenderer::defaultShader()
{
    ResourceManager::loadShader("shaders/particle.vert", "shaders/particle.frag", "particleShader");
    std::shared_ptr<ShaderProgram> shaderProgram(ResourceManager::getShader("particleShader"));

    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME,
                                                 UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    return shaderProgram;
}

void ParticleRenderer::render(std::vector<std::unique_ptr<Particle>> &particles,
                              const chag::float3 &position,
                              const std::shared_ptr<ParticleConf> &conf) {
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, conf->blendFunc);
    
    shaderProgram->backupCurrentShaderProgram();
    shaderProgram->use();
    
    texture->bind(GL_TEXTURE0);
    
    shaderProgram->setUniform3f("color", chag::make_vector(1.0f, 1.0f, 1.0f));
    shaderProgram->setUniform1i("sprite", 0);
    
    chag::float3 u = chag::normalize(camera->getUp());
    chag::float3 n = chag::normalize(camera->getLookAt() - camera->getPosition());
    chag::float3 r = chag::normalize(chag::cross(u, n));

    chag::float3 uprim = chag::cross(n, r);
    
    chag::float3x3 modelMatrix3x3 = make_matrix(r, uprim, n);
    
    float distance = length(camera->getPosition() - position);
    int maxParticles = (int)(particles.size() * LOD_FACTOR / distance );
    glBindVertexArray(vaob);
    

    // Sort the particles
    std::sort(particles.begin(), particles.end(), [this] (const std::unique_ptr<Particle> &p1,
                                                              const std::unique_ptr<Particle> &p2)
    {
        float l1 = length(camera->getPosition() - p1->getPosition());
        float l2 = length(camera->getPosition() - p2->getPosition());
    
        return l1 > l2;
    });

    // Render the particles
    int iterations = 0;

    chag::float3 scale = conf->calcParticleScale() * (1.0 + distance / LINEAR_SCALE_FACTOR);
    chag::float4x4 scaleMatrix = chag::make_scale<chag::float4x4>(scale);
    for (std::unique_ptr<Particle> &particle : particles) {
        if (iterations > maxParticles) { break; }
        iterations++;
    
        if (particle->isAlive()) {
            chag::float4x4 modelMatrix4x4 = make_matrix(modelMatrix3x3, particle->getPosition())
                                          * scaleMatrix;
             
            shaderProgram->setUniformMatrix4fv("modelMatrix", modelMatrix4x4);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    
    /* CLEANUP */
    glBindVertexArray(0);
    shaderProgram->restorePreviousShaderProgram();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    
    glEnable(GL_CULL_FACE);
}
