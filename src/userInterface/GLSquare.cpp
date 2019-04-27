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
#include <glutil/glutil.h>
#include <GL/glew.h>
#include <ShaderProgram.h>
#include "GLSquare.h"
#include <Texture.h>
#include <HUDGraphic.h>
#include <FontManager.h>
#include <IHudDrawable.h>
#include <ResourceManager.h>
#include <Globals.h>

void GLSquare::render(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix) {

    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    GLint blendSrc;
    GLint blendDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();

    bindTextureAndDraw(shaderProgram,projectionMatrix);

    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glBlendFunc(blendSrc, blendDst);

    shaderProgram->restorePreviousShaderProgram();
}

void GLSquare::bindTextureAndDraw(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix) {
    shaderProgram->use();
    glBindVertexArray(vao);

    shaderProgram->setUniform1i("sprite", 0);
    shaderProgram->setUniformMatrix4fv("modelMatrix", getModelMatrix());
    shaderProgram->setUniformMatrix4fv("projectionMatrix",*projectionMatrix);
    shaderProgram->setUniform1i("isFont",false);

    int* roundedCorners = graphic->getRoundedCorners();
    chag::float4 calcCorners = chag::make_vector(roundedCorners[0] / height,
                                                 roundedCorners[1] / height,
                                                 roundedCorners[2] / height,
                                                 roundedCorners[3] / height);
    shaderProgram->setUniform4f("roundedCorners",calcCorners);

    shaderProgram->setUniform1f("ratioWidthToHeight", width/height);

    int* borders = graphic->getBorders();
    chag::float4 calcBorders = chag::make_vector(borders[0] / height,
                                                 borders[1] / width,
                                                 borders[2] / height,
                                                 borders[3] / width);
    shaderProgram->setUniform4f("border",calcBorders);

    shaderProgram->setUniform4f("borderColor",graphic->getBorderColor());

    CHECK_GL_ERROR()

    if(graphic->isTextureElseColor()) {
        graphic->getTexture()->bind(GL_TEXTURE0);
        CHECK_GL_ERROR()
        shaderProgram->setUniform1i("isTexture",true);
        shaderProgram->setUniform1i("isColor",false);
    }else{
        shaderProgram->setUniform1i("isTexture",false);
        shaderProgram->setUniform1i("isColor",true);
        shaderProgram->setUniform4f("color",graphic->getColor());
    }

    CHECK_GL_ERROR()
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

chag::float4x4 GLSquare::getModelMatrix() {
    return chag::make_translation(originalPosition + relativePosition)
         * chag::make_rotation_z<chag::float4x4>(rotation)
         * chag::make_scale<chag::float4x4>(scale)
         * chag::make_translation(center)
         * chag::make_identity<chag::float4x4>();
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
    IHudDrawable::setRelativePosition(chag::make_vector(0.0f, 0.0f, 0.0f));
    center = image->getCenterOffset(width,height);
    updateOriginalPosition();
    fillVertexBuffer();
}

void GLSquare::updateOriginalPosition() {
    originalPosition = chag::make_vector( posX + width  / 2.0f - center.x,
                                         -posY - height / 2.0f - center.y,
                                          0.0f);
}

void GLSquare::setCenterOffset(chag::float3 offset) {
    center = offset;
    updateOriginalPosition();
}

void GLSquare::fillVertexBuffer() {

    float halfWidth = width/2, halfHeight = height/2;

    HUDGraphic::TexturePosition<float> tp = graphic->isTextureElseColor() ? graphic->getTexturePosition() : HUDGraphic::TexturePosition<float>(0.0f,0.0f,1.0f,1.0f);

    GLfloat quad[] = {
            -halfWidth, -halfHeight, 0.0f, tp.botLeftX, tp.botLeftY,0.0,0.0,
            halfWidth,  -halfHeight, 0.0f, tp.topRightX, tp.botLeftY,1.0,0.0,
            -halfWidth, halfHeight,  0.0f, tp.botLeftX, tp.topRightY,0.0,1.0,
            halfWidth,  halfHeight, 0.0f, tp.topRightX, tp.topRightY,1.0,1.0,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), ((char *)NULL + sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), ((char *)NULL + sizeof(float) * 5));

    // CLEANUP
    glBindVertexArray(0);
    CHECK_GL_ERROR();
}

GLSquare::~GLSquare() {
    glDeleteVertexArrays(1,&vao);
}

float GLSquare::getX() {
    return posX;
}

float GLSquare::getY() {
    return posY;
}

float GLSquare::getWidth(){
    return width;
}

float GLSquare::getHeight() {
    return height;
}

HUDGraphic* GLSquare::getGraphic(){
    return graphic;
}

void GLSquare::updateGraphic() {
    setCenterOffset(graphic->getCenterOffset(width,height));
    glDeleteVertexArrays(1,&vao);
    fillVertexBuffer();
}

void GLSquare::setGraphic(HUDGraphic *graphic) {
    this->graphic = graphic;
    updateGraphic();
}
