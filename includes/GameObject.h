#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Mesh.h"
#include "IDrawable.h"
#include <Triangle.h>
#include "IRenderComponent.h"
#include "Octree.h"

enum EventType {BeforeCollision, DuringCollision, AfterCollision};


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

    void addRenderComponent(IRenderComponent*);
    void addComponent(IComponent*);

    void update(float dt);
    void callEvent(EventType);

    AABB* getAABB();
    bool isDynamicObject();
    void setDynamic(bool);
    Octree* getOctree();

    static int uniqueId;
    int getId();
private:
    int id;
    Mesh *mesh;
    chag::float4x4 m_modelMatrix;

    /* COMPONENTS */
    IRenderComponent* renderComponent;
    std::vector<IComponent*> components;

    AABB aabb;
    bool dynamicObject = false;
    Octree *octree;

};


#endif
