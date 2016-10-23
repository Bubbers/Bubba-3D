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
#pragma once

#include <GL/glew.h>
#include <linmath/float4x4.h>
#include <IHudDrawable.h>
#include <memory>

class ShaderProgram;
class HUDGraphic;

/**
 * Represents a low level gl square with a HUDGraphic as its
 * graphical element
 */
class GLSquare : public IHudDrawable {
public:
    GLSquare(float posX, float posY, float width, float height, HUDGraphic* image);

    virtual ~GLSquare();

    float getX();
    float getY();
    float getWidth();
    float getHeight();
    HUDGraphic* getGraphic();

    void setCenterOffset(chag::float3 offset);
    void setGraphic(HUDGraphic* graphic);

    void updateGraphic();

    virtual void render(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix);

protected:
    float posX;
    float posY;
    float width;
    float height;

    HUDGraphic* graphic;
    GLuint vao;

    std::shared_ptr<ShaderProgram> shaderProgram;
    chag::float4x4* projectionMatrix;
    chag::float3 originalPosition;

    chag::float4x4 getModelMatrix();

    void init(float posX, float posY, float width, float height, HUDGraphic* image);
    void fillVertexBuffer();
    void bindTextureAndDraw(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix);
    void updateOriginalPosition();
};
