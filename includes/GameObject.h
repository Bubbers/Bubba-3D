#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include "IDrawable.h"
#include "GameObjectType.h"
#include "AABB2.h"
#include "float4x4.h"
#include <vector>

using namespace chag;

enum EventType {BeforeCollision, DuringCollision, AfterCollision};

class Mesh;
class Triangle;
class IRenderComponent;
class IComponent;
class Octree;

class GameObject : public IDrawable {
public:
    GameObject();
    GameObject(Mesh *mesh, GameObjectType type);
    ~GameObject();

    virtual void render();
    virtual void renderShadow(Shader* shaderProgram);

    void move(float4x4 model_matrix);
    void update(float4x4 update_matrix);

    void addRenderComponent(IRenderComponent*);
    void addComponent(IComponent*);

    void update(float dt);
    void callEvent(EventType, GameObjectType data);

    bool isDynamicObject();
    void setDynamic(bool);

    Octree* getOctree();
    std::vector<Triangle*> getTriangles();
    float4x4 getModelMatrix();
    AABB getAABB();

    int getId();

    void makeDirty();
    bool isDirty();

    GameObjectType getType() { return type; };
    void setType(GameObjectType t) { type = t; };

private:
    Octree* createOctree(Mesh* mesh);

    static int uniqueId;
    int id;
    Mesh *mesh;
    chag::float4x4 m_modelMatrix;

    GameObjectType type = SpaceEntity;

    IRenderComponent* renderComponent;
    std::vector<IComponent*> components;

    AABB aabb;
    Octree *octree;

    bool dynamicObject = false;

    bool dirty = false;
};


#endif
