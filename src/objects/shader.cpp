#include "shader.h"

Shader &Shader::use() {
  glUseProgram(Id);
  return *this;
};

void Shader::compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource){
GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // If geometry shader source code is given, also compile geometry shader
    if (geometrySource != NULL)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    Id = glCreateProgram();
    glAttachShader(Id, sVertex);
    glAttachShader(Id, sFragment);
    if (geometrySource != NULL)
        glAttachShader(Id, gShader);
    glLinkProgram(Id);
    checkCompileErrors(Id, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != NULL)
      glDeleteShader(gShader);
};

void Shader::setFloat(const GLchar *name, GLfloat value){
  glUniform1f(glGetUniformLocation(Id, name), value);
};

void Shader::setInt  (const GLchar *name, GLint   value){
  glUniform1i(glGetUniformLocation(Id, name), value);
};

void Shader::setVector2F(const GLchar *name, float2   &value){
  glUniform2f(glGetUniformLocation(Id, name), value.x, value.y);
};

void Shader::setVector3F(const GLchar *name, float3   &value){
  glUniform3f(glGetUniformLocation(Id, name), value.x, value.y, value.z);
};

void Shader::setVector4F(const GLchar *name, float4   &value){
  glUniform4f(glGetUniformLocation(Id, name), value.x, value.y, value.z, value.w);
}

void Shader::checkCompileErrors(GLuint object, std::string type){
  GLint success;
  GLchar infoLog[1024];
  Logger l = Logger::instance();
  if (type != "PROGRAM")
    {
      glGetShaderiv(object, GL_COMPILE_STATUS, &success);
      if (!success)
        {
	  glGetShaderInfoLog(object, 1024, NULL, infoLog);

	  l.logSevere("Error compiling shader" + string(infoLog));
        }
    }
  else
    {
      glGetProgramiv(object, GL_LINK_STATUS, &success);
      if (!success)
        {
	  glGetProgramInfoLog(object, 1024, NULL, infoLog);
	  l.logSevere("Error linking program" + string(infoLog));
        }
    }
};
