#include "SkyBoxRenderer.h"
#include "ResourceManager.h"
#include "constants.h"
#define SKYBOX_SHADER_NAME "skybox_shader"

SkyBoxRenderer::SkyBoxRenderer(Camera *camera, Mesh* skyMesh, float4x4* modelMatrix) : m_camera(camera), m_skyMesh(skyMesh), modelMatrix(modelMatrix) {
}

bool SkyBoxRenderer::init(const string &posXFilename, const string &negXFilename, const string &posYFilename,
                  const string &negYFilename, const string &posZFilename, const string &negZFilename) {
    m_pCubemap = new CubeMapTexture(posXFilename, negXFilename, posYFilename, negYFilename, posZFilename, negZFilename);
    ResourceManager::loadShader("../shaders/skybox.vert", "../shaders/skybox.frag", SKYBOX_SHADER_NAME);
    shaderProgram = ResourceManager::getShader(SKYBOX_SHADER_NAME);
    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    return true;
}

void SkyBoxRenderer::update(float dt) {
    *modelMatrix = make_translation(make_vector(0.0f, 2.0f, 0.0f)) * make_translation(m_camera->getPosition()) *
                        make_scale<float4x4>(make_vector(20.0f, 20.0f, 20.0f));
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
    shaderProgram->setUniformMatrix4fv("modelMatrix", *modelMatrix);

    for (size_t i = 0; i < m_skyMesh->m_chunks.size(); ++i) {
        Chunk &chunk = m_skyMesh->m_chunks[i];

        glBindVertexArray(chunk.m_vaob);
        glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, chunk.m_ind_bo);

        glDrawElements(GL_TRIANGLES, m_skyMesh->m_chunks[i].m_numIndices, GL_UNSIGNED_INT, 0);
        CHECK_GL_ERROR();
    }

    glCullFace(OldCullFaceMode);
    glDepthFunc(OldDepthFuncMode);
    shaderProgram->restorePreviousShaderProgram();
}


SkyBoxRenderer::~SkyBoxRenderer(){
    
}
