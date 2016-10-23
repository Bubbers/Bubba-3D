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

#include <IHudDrawable.h>
#include <GL/glew.h>
#include <vector>
#include <memory>

/**
 * A low-level OpenGL object that renders text within the specified dimensions.
 */
class TextObject : public IHudDrawable {

public:
    /**
     * Creates a text object that should be rendered at position (x,y). The
     * coordinates are screen-type 2D coordinates when used with the HudRenderer (see HudRenderer).
     */
    TextObject(std::string text, Font* font, int width, int height, int x, int y);

    ~TextObject() = default;
    /**
     * Renders the object using the specified shader program and projection
     * matrix. The shader should have the same variables as the default hud.vert
     * and hud.frag.
     */
    void virtual render(std::shared_ptr<ShaderProgram> shaderProgram, chag::float4x4* projectionMatrix);

    /**
     * Changes the text of the TextObject.
     */
    void virtual setText(std::string text);

protected:
    std::string text;
    Font* font;
    int width;
    int height;
    int x;
    int y;
    int numVertices;

    GLuint vao, vbo;
    bool buffersInitiated = false;

private:
    void addPoints(std::vector<GLfloat> *data, std::initializer_list<float> elems);
    void getLines(std::vector<std::string>* lines, std::vector<int>* linesOffsetLeft, int* numChars);
    int getOffsetByLineLength(int lineLength);
    void init(std::vector<std::string> lines, std::vector<int> linesOffsetLeft, int numChars);
    chag::float4x4 getModelMatrix();
    void initAndBindBuffers();

};
