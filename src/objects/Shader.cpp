#include "Shader.h"
#include "Logger.h"

#define MAX_LOG_SIZE 1024

void Shader::compile(const GLchar *vertexSource, GLchar *fragmentSource) {
    GLuint vertexShader, fragmentShader;
    compileShader(vertexShader  , GL_VERTEX_SHADER  ,vertexSource   , "VERTEX_SHADER");
    compileShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT_SHADER");
    createProgram(vertexShader, fragmentShader);
}

void Shader::compileShader(GLuint shader, GLenum type, const GLchar *source, std::string shaderType) {
    shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderType);
}


void Shader::checkCompileErrors(GLuint shader, std::string shaderType) {
    GLint successfullyCompiled;
    glGetProgramiv(shader, GL_COMPILE_STATUS, &successfullyCompiled);
    if(!successfullyCompiled) {
        logCompileError(shader, shaderType);
    }
}

void Shader::logCompileError(GLuint shader, std::string shaderType) {
    GLchar compileLog[MAX_LOG_SIZE];

    glGetShaderInfoLog(shader, MAX_LOG_SIZE, NULL, compileLog);
    Logger::logSevere("Failed to compile shader of type " + shaderType + "\n" + compileLog);
}

void Shader::createProgram(GLuint vertexShader, GLuint fragmentShader) {
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexShader);
    glAttachShader(this->shaderID, fragmentShader);

}

void Shader::linkProgram() {
    glLinkProgram(this->shaderID);
    checkLinkageErrors();
}

void Shader::checkLinkageErrors() {
    GLint successfullyLinked;
    glGetProgramiv(this->shaderID, GL_LINK_STATUS, &successfullyLinked);
    if(!successfullyLinked) {
        logLinkageError(this->shaderID);
    }
}

void Shader::logLinkageError(GLuint program) {
    GLchar linkageLog[MAX_LOG_SIZE];

    glGetShaderInfoLog(this->shaderID, MAX_LOG_SIZE, NULL, linkageLog);
    Logger::logSevere("Failed to compile shader of type PROGRAM\n" + *linkageLog);

}