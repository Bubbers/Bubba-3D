#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Mesh.h"
#include "IDrawable.h"

class GameObject : public IDrawable{
public:
  GameObject() {};
  GameObject(Mesh mesh);
  ~GameObject() { };

  virtual void render();

private:
  Mesh mesh;
};


#endif
