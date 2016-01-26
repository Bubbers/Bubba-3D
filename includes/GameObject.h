#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include "IDrawable.h"
#include "GameObjectType.h"
#include "AABB2.h"
#include "float4x4.h"
#include <vector>
#include <Quaternion.h>

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
    GameObject(Mesh *mesh, GameObjectType type, Mesh *colliderMesh);
    void initGameObject(Mesh *mesh, Mesh *colliderMesh, GameObjectType type);

    ~GameObject();

    int getUniqueId();

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


    float3 getScale();
    Quaternion getRotation();
    float3 getLocation();

    void setRotation(Quaternion r);
    void setLocation(float3 l);
    void setScale(float3 s);
    void updateRotation(Quaternion r);

    float getRadius();

private:
    Octree* createOctree(Mesh* mesh);


    float3 scale = make_vector(1.0f,1.0f,1.0f);
    Quaternion rotation = Quaternion();
    bool hasRotation = false;
    float3 location = make_vector(0.0f,0.0f,0.0f);
    bool changed = false;

    float radius = 0.0f;

    static int uniqueId;
    int id;
    Mesh *mesh;
    Mesh *collisionMesh;
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
