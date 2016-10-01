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
#include "ShaderProgram.h"
#include "Logger.h"
#include "VertexShader.h"

#define MAX_LOG_SIZE 1024

ShaderProgram::ShaderProgram() {

}

void ShaderProgram::loadShader(IShader *vertexShader, IShader *fragmentShader) {
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
    createProgram(vertexShader->getGLId(), fragmentShader->getGLId());
    linkProgram();
}

void ShaderProgram::createProgram(GLuint vertexShader, GLuint fragmentShader) {
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexShader);
    glAttachShader(this->shaderID, fragmentShader);

}

void ShaderProgram::linkProgram() {
    glLinkProgram(this->shaderID);
    checkLinkageErrors();
}

void ShaderProgram::checkLinkageErrors() {
    GLint successfullyLinked;
    glGetProgramiv(this->shaderID, GL_LINK_STATUS, &successfullyLinked);
    if(!successfullyLinked) {
        logLinkageError();
    }
}

void ShaderProgram::logLinkageError() {
    GLchar linkageLog[MAX_LOG_SIZE];

    glGetShaderInfoLog(this->shaderID, MAX_LOG_SIZE, NULL, linkageLog);
    Logger::logError("Failed to compile shader of type PROGRAM\n"
                     + std::to_string(*linkageLog));
}

void ShaderProgram::use() {
    glUseProgram(this->shaderID);
}

void ShaderProgram::backupCurrentShaderProgram() {
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShaderProgram);
}

void ShaderProgram::restorePreviousShaderProgram() {
    glUseProgram(previousShaderProgram);
}

GLint ShaderProgram::getUniformLocation(const std::string name) {
    std::map<std::string, GLint>::iterator it = uniformLocations.find(name);
    if(it != uniformLocations.end()) {
        return it->second;
    } else {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        uniformLocations.insert(std::pair<std::string,GLint>(name, location));
        return location;
    }
}

void ShaderProgram::setUniform1i(std::string name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setUniform1f(std::string name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setUniform3f(std::string name, chag::float3 value){
    glUniform3fv(getUniformLocation(name),1, &value.x);
}

void ShaderProgram::setUniform4f(std::string name, chag::float4 value) {
    glUniform4fv(getUniformLocation(name),1,&value.x);
}

void ShaderProgram::setUniformMatrix4fv(std::string name, chag::float4x4 matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix.c1.x);
}

void ShaderProgram::setUniform2f(std::string name, chag::float2 value) {
    glUniform2fv(getUniformLocation(name),1,&value.x);
}

void ShaderProgram::setUniformBufferObjectBinding(std::string bufferName, int index) {
    GLuint matricesUniform = glGetUniformBlockIndex(shaderID, bufferName.c_str());
    glUniformBlockBinding(shaderID, matricesUniform, index);
}

void ShaderProgram::initUniformBufferObject(std::string bufferName, int size, int index) {
    GLuint uniformBufferObject;
    glGenBuffers(1, &uniformBufferObject);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, index, uniformBufferObject, 0, size);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    uniformLocations.insert(std::pair<std::string, GLint>(bufferName, uniformBufferObject));
}

void ShaderProgram::setUniformBufferSubData(std::string bufferName, int offset, int size, const GLvoid *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, getUniformLocation(bufferName));
    glFinish();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

const char *textFileRead(const char *fn, bool fatalError) {

    FILE *fp;
    char *content = NULL;
    int count = 0;

    if (fn != NULL) {
        fp = fopen(fn, "rt");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            if (count > 0) {
                content = new char[count+1];
                count = fread( content, sizeof(char), count, fp);
                content[count] = '\0';
            } else {
                if (fatalError) {
                    char buffer[256];
                    sprintf(buffer, "File '%s' is empty\n", fn);
                    fatal_error(buffer);
                }
            }

            fclose(fp);
        } else {
            if (fatalError) {
                char buffer[256];
                sprintf(buffer, "Unable to read file '%s'\n", fn);
                fatal_error(buffer);
            }
        }
    } else {
        if (fatalError)
            fatal_error("textFileRead - argument NULL\n");
    }

    return content;
}



















