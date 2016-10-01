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

#include <map>
#include <string>
#include <linmath/float2.h>
#include "glutil/glutil.h"
#include "linmath/float3.h"
#include "IShader.h"

/**
 * \brief Class for maintaining OpenGL shader programs.
 *
 * The class maintains information about where the shaders are saved
 * on the graphics card. It caches the uniform locations as they are used to
 * speed up the next call.
 *
 * \code
 * ShaderProgram shaderProgram;
 * shaderProgram.loadShader(new VertexShader(vertexShaderString), new FragmentShader(fragmentShaderString));
 * shaderProgram.use();
 * \endcode
 */
class ShaderProgram {
public:
    ShaderProgram();

    /**
     * Compiles, links and creates a program of the specified shaders.
     */
    void loadShader(IShader *vertexShader, IShader *fragmentShader);

    /**
     * Activates the shader on the graphics card
     */
    void use();

    /**
     * Saves the shader currently active on the graphics card.
     * This is useful if later calculations may depend on the
     * currently active shader. Then you can save it, perform this
     * shader objects calculation, and restore the previous shader.
     */
    void backupCurrentShaderProgram();
    /**
     * Restores a shader saved by backupCurrentShaderProgram();
     */
    void restorePreviousShaderProgram();

    void setUniform1i(std::string name, int value);
    void setUniform1f(std::string name, float value);
    void setUniform2f(std::string name, chag::float2 value);
    void setUniform3f(std::string name, chag::float3 value);
    void setUniform4f(std::string name, chag::float4 value);
    void setUniformMatrix4fv(std::string name, chag::float4x4 matrix);

    void setUniformBufferObjectBinding(std::string bufferName, int index);
    void initUniformBufferObject(std::string bufferName, int size, int index);
    void setUniformBufferSubData(std::string bufferName, int offset, int size, const GLvoid *data);

private:

    void linkProgram();
    void checkLinkageErrors();
    void logLinkageError();

    void createProgram(GLuint vertexShader, GLuint fragmentShader);

    GLint getUniformLocation(std::string name);

    IShader* vertexShader;
    IShader* fragmentShader;

    GLuint shaderID;
    std::map<std::string, GLint> uniformLocations;
    GLint previousShaderProgram;

};
