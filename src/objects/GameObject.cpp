
#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "ResourceManager.h"
#include "constants.h"
#include "Collider.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

int GameObject::uniqueId = 0;

GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
    id = ++uniqueId;
}

GameObject::GameObject(Mesh *mesh) {
    GameObject();
    id = ++uniqueId;
    this->mesh = mesh;
    shininess = 0.0f;

    AABB* aabb = this->mesh->getAABB();
    std::vector<Triangle *> triangles;

    for (int i = 0; i < mesh->m_chunks.size(); i++) {

        for (int j = 0; j < mesh->m_chunks[i].m_positions.size(); j += 3) {


            Triangle *t = new Triangle(make_vector(mesh->m_chunks[i].m_positions[j + 0].x,
                                                   mesh->m_chunks[i].m_positions[j + 0].y,
                                                   mesh->m_chunks[i].m_positions[j + 0].z),
                                       make_vector(mesh->m_chunks[i].m_positions[j + 1].x,
                                                   mesh->m_chunks[i].m_positions[j + 1].y,
                                                   mesh->m_chunks[i].m_positions[j + 1].z),
                                       make_vector(mesh->m_chunks[i].m_positions[j + 2].x,
                                                   mesh->m_chunks[i].m_positions[j + 2].y,
                                                   mesh->m_chunks[i].m_positions[j + 2].z));


            triangles.push_back(t);
        }
    }



    float3 halfVector = (aabb->maxV - aabb->minV) / 2;
    float3 origin = aabb->maxV - halfVector;
    octree = new Octree(origin, halfVector, 0);
    octree->insertAll(triangles);

};

GameObject::~GameObject() {
    mesh = nullptr;

}

void GameObject::move(float4x4 model_matrix) {
    m_modelMatrix = model_matrix;
}

void GameObject::update(float4x4 update_matrix) {
    m_modelMatrix = m_modelMatrix * update_matrix;
}

void GameObject::render() {
    renderComponent->render();
}

std::vector<Triangle *> GameObject::getTriangles() {
    std::vector<Triangle *> ts;

    for (int i = 0; i < mesh->m_chunks.size(); i++) {

        for (int j = 0; j < mesh->m_chunks[i].m_positions.size(); j += 3) {


            Triangle *t = new Triangle(make_vector(mesh->m_chunks[i].m_positions[j + 0].x,
                                                   mesh->m_chunks[i].m_positions[j + 0].y,
                                                   mesh->m_chunks[i].m_positions[j + 0].z),

                                       make_vector(mesh->m_chunks[i].m_positions[j + 1].x,
                                                   mesh->m_chunks[i].m_positions[j + 1].y,
                                                   mesh->m_chunks[i].m_positions[j + 1].z),

                                       make_vector(mesh->m_chunks[i].m_positions[j + 2].x,
                                                   mesh->m_chunks[i].m_positions[j + 2].y,
                                                   mesh->m_chunks[i].m_positions[j + 2].z));
            ts.push_back(t);
        }
    }

    return ts;
}

float4x4* GameObject::getModelMatrix(){
    return &m_modelMatrix;
}


void GameObject::renderShadow(Shader *shaderProgram) {
    renderComponent->renderShadow(shaderProgram);
}

void GameObject::addRenderComponent(IRenderComponent* renderer){
    this->renderComponent = renderer;
    components.push_back(renderer);
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
}

void GameObject::update(float dt) {
    for(auto &component : components) {
        component->update(dt);
    }
}

void GameObject::callEvent(EventType type){
    switch(type) {
    case EventType::BeforeCollision:
        for (auto &component : components) {
            component->beforeCollision();
        }
        break;
    case EventType::DuringCollision:
        for (auto &component : components) {
            component->duringCollision();
        }
        break;
    case EventType::AfterCollision:
        for (auto &component : components) {
            component->afterCollision();
        }
        break;
    }
}

AABB* GameObject::getAABB(){
    AABB* meshAabb = this->mesh->getAABB();
    aabb = multiplyAABBWithModelMatrix(meshAabb, &this->m_modelMatrix);

    return &aabb;
}

bool GameObject::isDynamicObject(){
    return dynamicObject;
}

void GameObject::setDynamic(bool isDynamic) {
    dynamicObject = isDynamic;
}

Octree* GameObject::getOctree(){
    return octree;
}

int GameObject::getId() {
    return id;
}