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
//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H

#include <string>
#include <IShader.h>
#include <GL/glew.h>

#define VERTEX_SHADER_NAME_STRING "Vertex_Shader"

/**
 * \brief Compiles Vertex Shaders 
 *
 * Class that takes a vertex shader string and compiles it.
 * It also checks for any possible errors that occured during compilation.
 *
 */
class VertexShader : public IShader {
public:
    VertexShader() {};
    VertexShader(std::string shaderName);

    virtual void compile();
    virtual void checkErrors();
    virtual GLuint getGLId();

private:
    std::string shaderString;
    GLuint vertexShader;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H
