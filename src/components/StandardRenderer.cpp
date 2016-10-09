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
#include <GameObject.h>
#include <ShaderProgram.h>
#include <SFML/System/Clock.hpp>
#include "StandardRenderer.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "objects/Chunk.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

StandardRenderer::StandardRenderer(){

}

StandardRenderer::StandardRenderer(Mesh* mesh, GameObject* gameObject, ShaderProgram* shaderProgram):
        mesh(mesh), gameObject(gameObject)
{
    this->shaderProgram = shaderProgram;
}


void StandardRenderer::update(float dt){
    ;
}

void StandardRenderer::render() {
    shaderProgram->use();
    CHECK_GL_ERROR();

    chag::float4x4 modelMatrix = gameObject->getModelMatrix();

    chag::float4x4 normalMatrix = chag::inverse(chag::transpose(modelMatrix));
    shaderProgram->setUniformMatrix4fv("modelMatrix", modelMatrix);
    shaderProgram->setUniformMatrix4fv("normalMatrix", normalMatrix);

    for (size_t i = 0; i < mesh->getChunks()->size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = (*mesh->getChunks())[i];
        Material &material = (*mesh->getMaterials())[chunk.materialIndex];

        if (material.diffuseTexture != NULL) {
            shaderProgram->setUniform1i("diffuse_texture", DIFFUSE_TEXTURE_LOCATION);
            material.diffuseTexture->bind(GL_TEXTURE0);
        }
        if (material.bumpMapTexture != NULL) {
            shaderProgram->setUniform1i("normal_texture", NORMAL_TEXTURE_LOCATION);
            material.bumpMapTexture->bind(GL_TEXTURE3);
        }

        if(mesh->hasAnimations()) {
            float currentTimeInSeconds = clock.getElapsedTime().asSeconds();
            std::vector<chag::float4x4> boneTransforms = mesh->getBoneTransforms(currentTimeInSeconds);
            shaderProgram->setUniform1i("has_animations", 1);

            for (unsigned int j = 0; j < boneTransforms.size(); j++) {
                shaderProgram->setUniformMatrix4fv("bones[" + std::to_string(j) + "]", boneTransforms[j]);
            }
        } else {
            shaderProgram->setUniform1i("has_animations", 0);
        }

        shaderProgram->setUniform1i("has_diffuse_texture", material.diffuseTexture != NULL);
        shaderProgram->setUniform3f("material_diffuse_color", material.diffuseColor);
        //shaderProgram->setUniform3f("material_diffuse_color", chag::make_vector(0.5f, 0.5f, 0.5f));
        shaderProgram->setUniform3f("material_specular_color", material.specularColor);
        shaderProgram->setUniform3f("material_ambient_color", material.ambientColor);
        shaderProgram->setUniform3f("material_emissive_color", material.emissiveColor);
        shaderProgram->setUniform1i("has_normal_texture", material.bumpMapTexture != NULL);
        shaderProgram->setUniform1f("material_shininess", material.specularExponent);
        CHECK_GL_ERROR();

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, chunk.m_indices.size(), GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }
    CHECK_GL_ERROR();
}

void StandardRenderer::renderShadow(ShaderProgram *shaderProgram) {

    chag::float4x4 modelMatrix = gameObject->getModelMatrix();
    shaderProgram->setUniformMatrix4fv("modelMatrix", modelMatrix);

    for (size_t i = 0; i < mesh->getChunks()->size(); i++) {
        CHECK_GL_ERROR();

        Chunk &chunk = (*mesh->getChunks())[i];

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, chunk.m_indices.size(), GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    CHECK_GL_ERROR();
}
