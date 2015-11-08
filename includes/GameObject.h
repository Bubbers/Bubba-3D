#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Mesh.h"
#include "IDrawable.h"
#include "Shader.h"

class GameObject : public IDrawable{
public:
  GameObject() {};
  GameObject(Mesh mesh);
  ~GameObject() { };

  virtual void render();
    virtual void renderShadow(Shader shaderProgram);

  Mesh mesh;
private:

  
};


#endif
