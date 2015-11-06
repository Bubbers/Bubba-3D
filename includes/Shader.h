#ifndef __SHADER_H__
#define __SHADER_H__

#include "glutil/glutil.h"
#include <map>
#include "float3.h"

using namespace chag;

class Shader {
 public:
  Shader() { };
    GLuint shaderID;

    void loadShader(const std::string &vertexShader, const std::string &fragmentShader);

    void use();
    void backupCurrentShaderProgram();
    void restorePreviousShaderProgram();
    void setUniform1i(std::string name, int value);
    void setUniform1f(std::string name, float value);
    void setUniform3f(std::string name, float3 value);
    void setUniformMatrix4fv(std::string name, float4x4 matrix);

 private:
    std::map<std::string, GLint> uniformLocations;
    GLint previousShaderProgram;

    void compileAndLink(const GLchar *vertexSource, const GLchar *fragmentSource );
    void compileShaders(const GLchar *vertexSource, const GLchar *fragmentSource);
    void compileShader(GLuint* shader, GLenum type, const GLchar *source, std::string shaderType);
    void checkCompileErrors(GLuint *shader, std::string shaderType);
    void logCompileError(GLuint shader, std::string shaderType);
    void createProgram(GLuint vertexShader, GLuint fragmentShader);
    void linkProgram();
    void checkLinkageErrors();
    void logLinkageError();

    GLint getUniformLocation(std::string name);


};

#endif //__SHADER__H
