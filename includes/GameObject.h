#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Mesh.h"
#include "IDrawable.h"
#include "Shader.h"
#include <Triangle.h>

class GameObject : public IDrawable {
public:
    GameObject();

    GameObject(Mesh *mesh);

    ~GameObject();

    virtual void render();

    virtual void renderShadow(Shader* shaderProgram);

    void move(float4x4 model_matrix);
    void update(float4x4 update_matrix);
    std::vector<Triangle*> getTriangles();

private:
    Mesh *mesh;
    chag::float4x4 m_modelMatrix;
};


#endif
