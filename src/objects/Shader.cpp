#include "Shader.h"
#include "Logger.h"

#define MAX_LOG_SIZE 1024


void Shader::loadShader(const std::string &vertexShader, const std::string &fragmentShader) {
    const char *vs = textFileRead(vertexShader.c_str());
    const char *fs = textFileRead(fragmentShader.c_str());
    compileAndLink(vs,fs);
}

void Shader::compileAndLink(const GLchar *vertexSource, const GLchar *fragmentSource ) {
    compileShaders(vertexSource, fragmentSource);
    linkProgram();
}

void Shader::compileShaders(const char *vertexSource, const char *fragmentSource) {
    GLuint vertexShader, fragmentShader;
    compileShader(&vertexShader  , GL_VERTEX_SHADER  ,vertexSource   , "VERTEX_SHADER");
    compileShader(&fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT_SHADER");
    createProgram(vertexShader, fragmentShader);
}

void Shader::compileShader(GLuint *shader, GLenum type, const char *source, std::string shaderType) {
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    checkCompileErrors(shader, shaderType);
}


void Shader::checkCompileErrors(GLuint *shader, std::string shaderType) {
    GLint successfullyCompiled = false;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &successfullyCompiled);
    if(!successfullyCompiled) {
        logCompileError(*shader, shaderType);
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
        logLinkageError();
    }
}

void Shader::logLinkageError() {
    GLchar linkageLog[MAX_LOG_SIZE];

    glGetShaderInfoLog(this->shaderID, MAX_LOG_SIZE, NULL, linkageLog);
    Logger::logSevere("Failed to compile shader of type PROGRAM\n" + *linkageLog);
}

void Shader::use() {
    glUseProgram(this->shaderID);
}

void Shader::backupCurrentShaderProgram() {
    glGetIntegerv(GL_CURRENT_PROGRAM, &previousShaderProgram);
}

void Shader::restorePreviousShaderProgram() {
    glUseProgram(previousShaderProgram);
}

GLint Shader::getUniformLocation(const std::string name) {
    std::map<std::string, GLint>::iterator it = uniformLocations.find(name);
    if(it != uniformLocations.end()) {
        return it->second;
    } else {
        GLint location = glGetUniformLocation(shaderID, name.c_str());
        uniformLocations.insert(std::pair<std::string,GLint>(name, location));
        return location;
    }
}

void Shader::setUniform1i(std::string name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(std::string name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform3f(std::string name, float3 value){
    glUniform3fv(getUniformLocation(name),1, &value.x);
}

void Shader::setUniformMatrix4fv(std::string name, float4x4 matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix.c1.x);
}

void Shader::setUniformBufferObjectBinding(std::string bufferName, int index) {
    GLuint matricesUniform = glGetUniformBlockIndex(shaderID, bufferName.c_str());
    glUniformBlockBinding(shaderID, matricesUniform, index);
}

void Shader::initUniformBufferObject(std::string bufferName, int size, int index) {
    GLuint uniformBufferObject;
    glGenBuffers(1, &uniformBufferObject);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, index, uniformBufferObject, 0, size);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    uniformLocations.insert(std::pair<std::string, GLint>(bufferName, uniformBufferObject));
}

void Shader::setUniformBufferSubData(std::string bufferName, int offset, int size, const GLvoid *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, getUniformLocation(bufferName));
    glFinish();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}



















