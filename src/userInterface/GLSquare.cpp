//
// Created by simon on 2016-02-02.
//

#include <glutil/glutil.h>
#include <GL/glew.h>
#include <ShaderProgram.h>
#include "GLSquare.h"
#include <Texture.h>
#include <HUDGraphic.h>
#include <FontManager.h>
#include <IHudDrawable.h>

void GLSquare::render(ShaderProgram* shaderProgram, float4x4* projectionMatrix) {

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();

    bindTextureAndDraw(shaderProgram,projectionMatrix);

    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    shaderProgram->restorePreviousShaderProgram();
}

void GLSquare::bindTextureAndDraw(ShaderProgram *shaderProgram, float4x4* projectionMatrix) {
    shaderProgram->use();
    glBindVertexArray(vao);

    shaderProgram->setUniform1i("sprite", 0);
    shaderProgram->setUniformMatrix4fv("modelMatrix", getModelMatrix());
    shaderProgram->setUniformMatrix4fv("projectionMatrix",*projectionMatrix);
    shaderProgram->setUniform1i("isFont",false);

    if(graphic->isTextureElseColor()) {
        graphic->getTexture()->bind(GL_TEXTURE0);
        shaderProgram->setUniform1i("isTexture",true);
        shaderProgram->setUniform1i("isColor",false);
    }else{
        shaderProgram->setUniform1i("isTexture",false);
        shaderProgram->setUniform1i("isColor",true);
        shaderProgram->setUniform4f("color",graphic->getColor());
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

float4x4 GLSquare::getModelMatrix() {
    return make_translation(originalPosition+relativePosition)*make_rotation_z<float4x4>(rotation)*make_translation(center)*make_identity<float4x4>();
}

GLSquare::GLSquare(float posX, float posY, float width, float height, HUDGraphic *image) {
    init(posX,posY,width,height,image);
}

void GLSquare::init(float posX, float posY, float width, float height, HUDGraphic *image) {
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;
    this->graphic = image;
    IHudDrawable::setRelativePosition(make_vector(0.0f,0.0f,0.0f));
    center = image->getCenterOffset(width,height);
    updateOriginalPosition();
    fillVertexBuffer();
}

void GLSquare::updateOriginalPosition() {
    originalPosition = make_vector(posX+width/2.0f+center.x,-posY-height/2.0f-center.y,0.0f);
}

void GLSquare::setCenterOffset(float3 offset) {
    center = offset;
    updateOriginalPosition();
}

void GLSquare::fillVertexBuffer() {

    float halfWidth = width/2, halfHeight = height/2;

    GLfloat quad[] = {
            -halfWidth, -halfHeight, 0.0f, 0.0f, 0.0f,
            halfWidth,  -halfHeight, 0.0f, 1.0f, 0.0f,
            halfWidth,  halfHeight, 0.0f, 1.0f, 1.0f,

            -halfWidth, -halfHeight,  0.0f, 0.0f, 0.0f,
            halfWidth,  halfHeight, 0.0f, 1.0f, 1.0f,
            -halfWidth, halfHeight,  0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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

GLSquare::~GLSquare() {
    glDeleteVertexArrays(1,&vao);
}