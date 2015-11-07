#ifndef __IDRAWABLE_H__
#define __IDRAWABLE_H__

#include "Shader.h"


class IDrawable {
public:
	IDrawable() { shininess = 0.0f; };
  virtual ~IDrawable() {};
  virtual void render() = 0;

  
    float shininess;
	Shader shaderProgram;

	void setShaderProgram(Shader shaderProgram) {
		this->shaderProgram = shaderProgram;
	}
};


#endif
