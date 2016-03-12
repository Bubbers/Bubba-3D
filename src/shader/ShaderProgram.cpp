#include "ShaderProgram.h"
#include "Logger.h"

#define MAX_LOG_SIZE 1024

ShaderProgram::ShaderProgram() {

}

void ShaderProgram::loadShader(const std::string &vertexShader, const std::string &fragmentShader) {
    const char *vs = textFileRead(vertexShader.c_str());
    const char *fs = textFileRead(fragmentShader.c_str());
    compileAndLink(vs,fs);
}

void ShaderProgram::compileAndLink(const GLchar *vertexSource, const GLchar *fragmentSource ) {
    compileShaders(vertexSource, fragmentSource);
    linkProgram();
}

void ShaderProgram::compileShaders(const char *vertexSource, const char *fragmentSource) {
    GLuint vertexShader, fragmentShader;
    compileShader(&vertexShader  , GL_VERTEX_SHADER  ,vertexSource   , "VERTEX_SHADER");
    compileShader(&fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, "FRAGMENT_SHADER");
    createProgram(vertexShader, fragmentShader);
}

void ShaderProgram::compileShader(GLuint *shader, GLenum type, const char *source, std::string shaderType) {
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    checkCompileErrors(shader, shaderType);
}


void ShaderProgram::checkCompileErrors(GLuint *shader, std::string shaderType) {
    GLint successfullyCompiled = false;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &successfullyCompiled);
    if(!successfullyCompiled) {
        logCompileError(*shader, shaderType);
    }
}

void ShaderProgram::logCompileError(GLuint shader, std::string shaderType) {
    GLchar compileLog[MAX_LOG_SIZE];

    glGetShaderInfoLog(shader, MAX_LOG_SIZE, NULL, compileLog);
    Logger::logError("Failed to compile shader of type " + shaderType + "\n" + compileLog);
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
    Logger::logError("Failed to compile shader of type PROGRAM\n" + *linkageLog);
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

void ShaderProgram::setUniform3f(std::string name, float3 value){
    glUniform3fv(getUniformLocation(name),1, &value.x);
}

void ShaderProgram::setUniformMatrix4fv(std::string name, float4x4 matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, false, &matrix.c1.x);
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



















