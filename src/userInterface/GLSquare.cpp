//
// Created by simon on 2016-02-02.
//

#include <glutil/glutil.h>
#include <GL/glew.h>
#include <Shader.h>
#include "GLSquare.h"
#include <Texture.h>

void GLSquare::render(Shader* shaderProgram, GLuint* vertexBuffer) {
    fillVertexBuffer(vertexBuffer);

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();

    bindTextureAndDraw(shaderProgram,vertexBuffer);

    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    shaderProgram->restorePreviousShaderProgram();
}

void GLSquare::bindTextureAndDraw(Shader *shaderProgram, GLuint* vertexBuffer) {
    shaderProgram->use();
    glBindVertexArray(*vertexBuffer);

    shaderProgram->setUniform1i("sprite", 0);
    shaderProgram->setUniformMatrix4fv("modelMatrix", getModelMatrix());

    texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

//TODO Add support for modifiable layouts
float4x4 GLSquare::getModelMatrix() {
    return make_identity<float4x4>();
}

GLSquare::GLSquare(float posX, float posY, float width, float height, Texture *image) {
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;
    this->texture = image;
}

void GLSquare::fillVertexBuffer(GLuint* vertexBuffer) {

    GLfloat quad[] = {
            posX,       -posY-height, 0.0f, 0.0f, 0.0f,
            posX+width, -posY,        0.0f, 1.0f, 1.0f,
            posX,       -posY,        0.0f, 0.0f, 1.0f,

            posX,       -posY-height, 0.0f, 0.0f, 0.0f,
            posX+width, -posY-height, 0.0f, 1.0f, 0.0f,
            posX+width, -posY,        0.0f, 1.0f, 1.0f,
    };

    glBindVertexArray(*vertexBuffer);

    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // CLEANUP
    glBindVertexArray(0);
    CHECK_GL_ERROR();
}