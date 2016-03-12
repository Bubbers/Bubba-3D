//
// Created by johan on 2016-03-12.
//

#include "VertexShader.h"
#include <string>
#include <GL/glew.h>
#include <Utils.h>


VertexShader::VertexShader(std::string shaderName) {
    shaderString = textFileRead(shaderName.c_str(), true);
    compile();
    checkErrors();
}

void VertexShader::compile() {
    vertexShader = compileShader(GL_VERTEX_SHADER, shaderString.c_str());
    checkErrors();
}

void VertexShader::checkErrors() {
    checkCompileErrors(&vertexShader, VERTEX_SHADER_NAME_STRING);
}

GLuint VertexShader::getGLId() {
    return this->vertexShader;
}
