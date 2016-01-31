#ifndef __IDRAWABLE_H__
#define __IDRAWABLE_H__

class Shader;

class IDrawable {
public:
	IDrawable() { shininess = 0.0f; };

	virtual ~IDrawable(){};

  virtual void render() = 0;
  virtual void renderShadow(Shader* shaderProgram) = 0;
  
  float shininess;
};


#endif
