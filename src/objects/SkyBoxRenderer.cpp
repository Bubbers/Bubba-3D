#include "SkyBoxRenderer.h"
#include "ResourceManager.h"
#include "constants.h"
#include "Camera.h"
#include "CubeMapTexture.h"
#include "GameObject.h"

#define SKYBOX_SHADER_NAME "skybox_shader"

SkyBoxRenderer::SkyBoxRenderer(Camera *camera, Mesh* skyMesh, GameObject* gameObject) : m_camera(camera), m_skyMesh(skyMesh), gameObject(gameObject) {
}

bool SkyBoxRenderer::init(const string &posXFilename, const string &negXFilename, const string &posYFilename,
                  const string &negYFilename, const string &posZFilename, const string &negZFilename) {
    m_pCubemap = new CubeMapTexture(posXFilename, negXFilename, posYFilename, negYFilename, posZFilename, negZFilename);
    ResourceManager::loadShader("shaders/skybox.vert", "shaders/skybox.frag", SKYBOX_SHADER_NAME);
    shaderProgram = ResourceManager::getShader(SKYBOX_SHADER_NAME);
    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    return true;
}

void SkyBoxRenderer::update(float dt) {
    gameObject->move(make_translation(make_vector(0.0f, 2.0f, 0.0f)) * make_translation(m_camera->getPosition()) *
                     make_scale<float4x4>(make_vector(10000.0f, 10000.0f, 10000.0f)));
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

    float4x4 modelMatrix = gameObject->getModelMatrix();
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
