#ifndef __IDRAWABLE_H__
#define __IDRAWABLE_H__


class IDrawable {
public:
  virtual ~IDrawable() {};
  virtual void render() = 0;
  
  float shininess;
};


#endif
