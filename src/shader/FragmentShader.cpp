//
// Created by johan on 2016-03-12.
//

#include "FragmentShader.h"

#define FRAGMENT_SHADER_NAME_STRING "Vertex_Shader"

FragmentShader::FragmentShader(std::string shaderName) {
    shaderString = textFileRead(shaderName.c_str(), true);
    compile();
    checkErrors();
}

void FragmentShader::compile() {
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, shaderString.c_str());
    checkErrors();
}

void FragmentShader::checkErrors() {
    checkCompileErrors(&fragmentShader, FRAGMENT_SHADER_NAME_STRING);
}

GLuint FragmentShader::getGLId() {
    return this->fragmentShader;
}

