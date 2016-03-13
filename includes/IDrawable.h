#ifndef __IDRAWABLE_H__
#define __IDRAWABLE_H__

class ShaderProgram;

class IDrawable {
public:
	IDrawable() { shininess = 0.0f; };

	virtual ~IDrawable(){};

  virtual void render() = 0;
  virtual void renderShadow(ShaderProgram* shaderProgram) = 0;
  
  float shininess;
};


#endif
