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

#ifndef SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_ISHADER_H

#include <string>
#include "Logger.h"
#include "glutil/glutil.h"

#define MAX_LOG_SIZE 1024

class IShader {
public:
    virtual void compile() = 0;
    virtual void checkErrors() = 0;
    virtual GLuint getGLId() = 0;

    GLuint compileShader(GLenum type, const char *source) {
        GLuint compiledShader = glCreateShader(type);
        glShaderSource(compiledShader, 1, &source, NULL);
        glCompileShader(compiledShader);
        return compiledShader;
    }

    void logCompileError(GLuint shader, std::string shaderType) {
        GLchar compileLog[MAX_LOG_SIZE];

        glGetShaderInfoLog(shader, MAX_LOG_SIZE, NULL, compileLog);
        Logger::logError("Failed to compile shader of type " + shaderType + "\n" + compileLog);
    }

    void checkCompileErrors(GLuint *shader, std::string shaderType) {
        GLint successfullyCompiled = false;
        glGetShaderiv(*shader, GL_COMPILE_STATUS, &successfullyCompiled);
        if (!successfullyCompiled) {
            logCompileError(*shader, shaderType);
        }
    }


};


#endif //SUPER_BUBBA_AWESOME_SPACE_ISHADER_H
