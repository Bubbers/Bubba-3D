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
#include "constants.h"
#include "linmath/float3x3.h"
#include "Camera.h"
#include "Particle.h"
#include "ParticleConf.h"


ParticleGenerator::ParticleGenerator(Texture *texture, int amount,
                                     Camera *camera, ParticleConf *conf)
                                   : texture(texture), m_amount(amount),
                                     m_camera(camera), conf(conf)
{
    ResourceManager::loadShader("shaders/particle.vert", "shaders/particle.frag", "particleShader");
    shaderProgram = ResourceManager::getShader("particleShader");
    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME,
                                                 UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);
    GLfloat quad[] = { //POSITION3 TEXCOORD2
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };
    
    glGenVertexArrays(1, &m_vaob);
    glBindVertexArray(m_vaob);
    
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


ParticleGenerator::~ParticleGenerator()
{

}

void ParticleGenerator::setScaleLod(bool value) {
    doScale = value;
}



void ParticleGenerator::render() {
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, conf->blendFunc);
    
    shaderProgram->backupCurrentShaderProgram();
    shaderProgram->use();
    
    texture->bind(GL_TEXTURE0);
    
    shaderProgram->setUniform3f("color", chag::make_vector(1.0f, 1.0f, 1.0f));
    shaderProgram->setUniform1i("sprite", 0);
    
    chag::float3x3 modelMatrix3x3 = getModelMatrix3x3();
    
    float distance = length(m_camera->getPosition() - owner->getAbsoluteLocation());
    int maxParticles = (int)(m_amount * LOD_FACTOR / distance );
    glBindVertexArray(m_vaob);
    

    // Sort the particles
    std::sort(m_particles.begin(), m_particles.end(), [this] (const std::unique_ptr<Particle> &p1,
                                                              const std::unique_ptr<Particle> &p2)
    {
        float l1 = length(m_camera->getPosition() - p1->getPosition());
        float l2 = length(m_camera->getPosition() - p2->getPosition());
    
        return l1 > l2;
    });

    // Render the particles
    int iterations = 0;
    for (std::unique_ptr<Particle> &particle : m_particles) {
        if (iterations > maxParticles) { break; }
        iterations++;
    
        chag::float3 scale;
        if (particle->isAlive()) {
            if(doScale) {
                scale = conf->calcParticleScale() * (1.0 + distance / LINEAR_SCALE_FACTOR);
            } else {
                scale = chag::make_vector(1.0f, 1.0f, 1.0f);
            }
            chag::float4x4 modelMatrix4x4 = make_matrix(modelMatrix3x3, particle->getPosition())
                                                      * chag::make_scale<chag::float4x4>(scale);
    
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

void ParticleGenerator::update(float dt) {
    if (m_particles.empty() && owner != nullptr) {
        for (int i = 0; i < m_amount; i++) {
            std::unique_ptr<Particle> part(new Particle(*conf, owner->getModelMatrix()));
            m_particles.push_back(std::move(part));
        }
    }

    float distance = length(m_camera->getPosition() - owner->getAbsoluteLocation());

    for (std::unique_ptr<Particle> &particle : m_particles) {
        if (particle->isAlive()){
            particle->update(dt, distance, *conf);
        }
        else if(conf->loop(dt)){
            particle->reset(*conf, owner->getModelMatrix());
        }
    }
}

chag::float3x3 ParticleGenerator::getModelMatrix3x3() {
    chag::float3 u = chag::normalize(m_camera->getUp());
    chag::float3 n = chag::normalize(m_camera->getLookAt() - m_camera->getPosition());
    chag::float3 r = chag::normalize(chag::cross(u, n));

    chag::float3 uprim = chag::cross(n, r);
    
    return make_matrix(r, uprim, n);
}
