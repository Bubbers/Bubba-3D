#ifndef __SHADER_H__
#define __SHADER_H__

#include "glutil/glutil.h"

class Shader {
 public:
  Shader() { };

  void compile(const GLchar *vertexSource, GLchar *fragmentSource);

 private:
  GLuint shaderID;

    void compileShader(GLuint shader, GLenum type, const GLchar *source, std::string shaderType);
    void checkCompileErrors(GLuint shader, std::string shaderType);
    void logCompileError(GLuint shader, std::string shaderType);
    void createProgram(GLuint vertexShader, GLuint fragmentShader);
    void linkProgram();
    void checkLinkageErrors();
    void logLinkageError(GLuint program);
};

#endif //__SHADER__H
