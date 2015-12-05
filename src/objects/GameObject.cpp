
#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "ResourceManager.h"
#include "constants.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0


GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
}

GameObject::GameObject(Mesh *mesh) {
    GameObject();
    this->mesh = mesh;
    shininess = 0.0f;
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

            float4 p1 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 0].x,
                                                    mesh->m_chunks[i].m_positions[j + 0].y,
                                                    mesh->m_chunks[i].m_positions[j + 0].z, 1.0f);

            float4 p2 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 1].x,
                                                    mesh->m_chunks[i].m_positions[j + 1].y,
                                                    mesh->m_chunks[i].m_positions[j + 1].z, 1.0f);

            float4 p3 = m_modelMatrix * make_vector(mesh->m_chunks[i].m_positions[j + 2].x,
                                                    mesh->m_chunks[i].m_positions[j + 2].y,
                                                    mesh->m_chunks[i].m_positions[j + 2].z, 1.0f);

            Triangle *t = new Triangle(make_vector(p1.x, p1.y, p1.z), make_vector(p2.x, p2.y, p2.z),
                                       make_vector(p3.x, p3.y, p3.z));

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
    case EventType::OnCollision:
        for (auto &component : components) {
            component->onCollision();
        }
        break;
    }
}