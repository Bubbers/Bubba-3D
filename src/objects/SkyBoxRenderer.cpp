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
#include "SkyBoxRenderer.h"
#include "ResourceManager.h"
#include "constants.h"
#include "Camera.h"
#include "CubeMapTexture.h"
#include "GameObject.h"
#include "Chunk.h"

#define SKYBOX_SHADER_NAME "skybox_shader"

SkyBoxRenderer::SkyBoxRenderer(Camera *camera, Mesh* skyMesh, GameObject* gameObject) : m_camera(camera), m_skyMesh(skyMesh), gameObject(gameObject) {
}

bool SkyBoxRenderer::init(const std::string &posXFilename, const std::string &negXFilename,
                          const std::string &posYFilename, const std::string &negYFilename,
                          const std::string &posZFilename, const std::string &negZFilename)
{
    m_pCubemap = new CubeMapTexture(posXFilename, negXFilename, posYFilename, negYFilename, posZFilename, negZFilename);
    ResourceManager::loadShader("shaders/skybox.vert", "shaders/skybox.frag", SKYBOX_SHADER_NAME);
    shaderProgram = ResourceManager::getShader(SKYBOX_SHADER_NAME);
    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    return true;
}

void SkyBoxRenderer::update(float dt) {
    gameObject->move(chag::make_translation(chag::make_vector(0.0f, 2.0f, 0.0f)) * chag::make_translation(m_camera->getPosition()) *
                     chag::make_scale<chag::float4x4>(chag::make_vector(10000.0f, 10000.0f, 10000.0f)));
}

void SkyBoxRenderer::render() {
    shaderProgram->backupCurrentShaderProgram();
    shaderProgram->use();

    GLint OldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    GLint OldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    m_pCubemap->bind(GL_TEXTURE0);
    shaderProgram->setUniform1i("cubeMapSampler", 0);

    chag::float4x4 modelMatrix = gameObject->getModelMatrix();
    shaderProgram->setUniformMatrix4fv("modelMatrix", modelMatrix);

    for (size_t i = 0; i < m_skyMesh->getChunks()->size(); i++) {
        renderChunk((*m_skyMesh->getChunks())[i]);
    }

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
    shaderProgram->restorePreviousShaderProgram();
}


void SkyBoxRenderer::renderChunk(Chunk& chunk) {
    glBindVertexArray(chunk.m_vaob);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

    glDrawElements(GL_TRIANGLES, chunk.m_indices.size(), GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
}

SkyBoxRenderer::~SkyBoxRenderer(){

}
