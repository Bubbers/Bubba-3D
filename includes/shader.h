#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

#include <float2.h>
#include <float3.h>
#include <string>
#include "float4x4.h"
#include "Logger.h"

using namespace chag;

class Shader {
 public:
  
  Shader() {}
  Shader &use();

  void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource);
  void setFloat(const GLchar *name, GLfloat value);
  void setInt  (const GLchar *name, GLint   value);

  void setVector2F(const GLchar *name, float2   &value);
  void setVector3F(const GLchar *name, float3   &value);
  void setVector4F(const GLchar *name, float4   &value);
  void setMatrix4 (const GLchar *name, float4x4 &value);
 private:
  GLuint Id;
  void checkCompileErrors(GLuint object, std::string type);
};

#endif
