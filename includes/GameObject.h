#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include "IDrawable.h"
#include "AABB2.h"
#include "float4x4.h"
#include <vector>
#include <Quaternion.h>
#include <Sphere.h>

using namespace chag;
using namespace std;

enum EventType {BeforeCollision, DuringCollision, AfterCollision};

typedef int Identifier;

class Mesh;
class Triangle;
class IRenderComponent;
class IComponent;
class Octree;

class GameObject : public IDrawable {
public:
    GameObject();
    GameObject(Mesh *mesh);
    GameObject(Mesh *mesh, Mesh *colliderMesh);
    void initGameObject(Mesh *mesh, Mesh *colliderMesh);

    virtual ~GameObject();

    int getUniqueId();

    virtual void render();
    virtual void renderShadow(Shader* shaderProgram);

    void move(float4x4 model_matrix);
    void update(float4x4 update_matrix);

    void addRenderComponent(IRenderComponent*);
    void addComponent(IComponent*);

    void update(float dt);
    void callEvent(EventType, GameObject* data);

    bool isDynamicObject();
    void setDynamic(bool);

    Identifier getIdentifier();
    void setIdentifier(Identifier identifier);
    void addCollidesWith(Identifier colliderID);
    void addCollidesWith(initializer_list<Identifier> colliderIDs);
    void clearCollidesWithList();
    bool collidesWith(Identifier id);

    Octree* getOctree();
    std::vector<Triangle*> getTriangles();
    float4x4 getModelMatrix();
    AABB getAABB();

    int getId();

    void makeDirty();
    bool isDirty();

    float3 getScale();
    Quaternion getRotation();
    float3 getLocation();

    void setRotation(Quaternion r);
    void setLocation(float3 l);
    void setScale(float3 s);
    void updateRotation(Quaternion r);

    Sphere getSphere();

private:
    Octree* createOctree(Mesh* mesh);
    Identifier identifier = -1;
    std::vector<Identifier> canCollideWith;

    float3 scale = make_vector(1.0f,1.0f,1.0f);
    Quaternion rotation = Quaternion();
    bool hasRotation = false;
    float3 location = make_vector(0.0f,0.0f,0.0f);
    bool changed = false;

    Sphere sphere;

    static int uniqueId;
    int id;
    Mesh *mesh;
    Mesh *collisionMesh;
    chag::float4x4 m_modelMatrix;

    IRenderComponent* renderComponent;
    std::vector<IComponent*> components;

    AABB aabb;
    Octree *octree;

    bool dynamicObject = false;

    bool dirty = false;
};


#endif
