#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Mesh.h"
#include "IDrawable.h"
#include "Shader.h"
#include <Triangle.h>
#include "IRenderComponent.h"

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

    float4x4* getModelMatrix();
    Shader* getShaderProgram();

    void addRenderComponent(IRenderComponent*);
    IRenderComponent* renderComponent;

private:
    Mesh *mesh;
    chag::float4x4 m_modelMatrix;
};


#endif
