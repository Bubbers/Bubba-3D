//
// Created by johan on 2015-12-12.
//

#include <ResourceManager.h>
#include "HudRenderer.h"

HudRenderer::HudRenderer(){
    ResourceManager::loadShader("../shaders/hud.vert", "../shaders/hud.frag", "hudShader");
    shaderProgram = ResourceManager::getShader("hudShader");

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
    CHECK_GL_ERROR();
}

HudRenderer::~HudRenderer(){

}

void HudRenderer::render() {
    float4x4 modelMat = make_identity<float4x4>();
    Texture* texture1 = ResourceManager::loadAndFetchTexture("../scenes/negx.jpg");
    render2DHud(texture1, &modelMat);

    modelMat = make_translation(make_vector(-1.0f, -1.0f, -0.5f)) * make_scale<float4x4>(make_vector(1.0f, 1.0f, 1.0f));
    Texture* texture = ResourceManager::loadAndFetchTexture("../scenes/blending_transparent_window.png");
    render2DHud(texture, &modelMat);
}

void HudRenderer::render2DHud(Texture* texture, float4x4 *modelMatrix) {
    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();
    shaderProgram->use();
    glBindVertexArray(m_vaob);

    shaderProgram->setUniform1i("sprite", 0);
    shaderProgram->setUniformMatrix4fv("modelMatrix", *modelMatrix);

    texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    shaderProgram->restorePreviousShaderProgram();
}


void HudRenderer::renderShadow(Shader *shaderProgram) {}

void HudRenderer::update(float dt){

}