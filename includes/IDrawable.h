#ifndef __IDRAWABLE_H__
#define __IDRAWABLE_H__


class IDrawable {
public:
	IDrawable() { shininess = 0.0f; };
  virtual ~IDrawable() {};
  virtual void render() = 0;
  void setupUniforms() {};
  
  float shininess;
};


#endif
