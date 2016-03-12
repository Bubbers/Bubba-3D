
#include <algorithm>
#include <vector>
#include <Logger.h>
#include "linmath/Quaternion.h"
#include "Sphere.h"
#include "GameObject.h"
#include "linmath/float3x3.h"
#include "Collider.h"
#include "Mesh.h"
#include "IComponent.h"
#include "IRenderComponent.h"
#include "Octree.h"



#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

int GameObject::uniqueId = 0;

GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
    id = getUniqueId();
}

GameObject::GameObject(Mesh *mesh) {
    m_modelMatrix = make_identity<float4x4>();
    initGameObject(mesh, mesh);
};

GameObject::GameObject(Mesh *mesh, Mesh *colliderMesh) {
    m_modelMatrix = make_identity<float4x4>();
    initGameObject(mesh, colliderMesh);
}


void GameObject::initGameObject(Mesh *mesh, Mesh *colliderMesh) {
    this->mesh = mesh;
    this->collisionMesh = colliderMesh;
    this->m_modelMatrix = make_identity<float4x4>();
    this->shininess = 0.0f;
    this->id = getUniqueId();
    this->sphere = mesh->getSphere();
    this->octree = createOctree(colliderMesh);
}

GameObject::~GameObject() {
    mesh = nullptr;
}

int GameObject::getUniqueId() {
    return ++uniqueId;
}

Octree* GameObject::createOctree(Mesh* mesh) {
    AABB* aabb = this->mesh->getAABB();
    float3 halfVector = (aabb->maxV - aabb->minV) / 2;
    float3 origin = aabb->maxV - halfVector;
    Octree* octree = new Octree(origin, halfVector);

    std::vector<Triangle *> triangles = mesh->getTriangles();
    octree->insertAll(triangles);

    return octree;
}

Sphere GameObject::getTransformedSphere() {
    float scaling = max(scale.x, max(scale.y, scale.z));
    return Sphere(sphere.getPosition()+location, scaling*sphere.getRadius());
}

void GameObject::makeDirty() {
    for (auto &component : components) {
        component->onDeath();
    }
    dirty = true;
}

bool GameObject::isDirty() {
    return dirty;
}

void GameObject::move(float4x4 modelMatrix) {
    m_modelMatrix = modelMatrix;
}

void GameObject::update(float4x4 updateMatrix) {
    m_modelMatrix = m_modelMatrix * updateMatrix;
}

void GameObject::render() {
    if(renderComponent != nullptr) {
        renderComponent->render();
    }
	if (children.size() != 0) {
		for (auto child : children) {
			child.first->renderChild(m_modelMatrix * child.second);
		}
	}
}

void GameObject::renderChild(float4x4 offsetMatrix) {
	float4x4 temp = m_modelMatrix;
	m_modelMatrix = m_modelMatrix * offsetMatrix;
	render();
	m_modelMatrix = temp;
}

std::vector<Triangle *> GameObject::getTriangles() {
    return mesh->getTriangles();
}

float4x4 GameObject::getModelMatrix() {
    return m_modelMatrix;
}


void GameObject::renderShadow(ShaderProgram *shaderProgram) {
    renderComponent->renderShadow(shaderProgram);
}

void GameObject::addRenderComponent(IRenderComponent* renderer) {
    this->renderComponent = renderer;
    components.push_back(renderer);
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
}

void GameObject::update(float dt) {
    for (auto &component : components) {
        component->update(dt);
    }
    if (changed) {
        changed = false;
        float4x4 transform = make_translation(location);
        if (hasRotation) {
            transform = transform * makematrix(rotation);
        }
        move(transform*make_scale<float4x4>(scale));
    }
}

void GameObject::callEvent(EventType type, GameObject* data) {
    switch (type) {
    case EventType::BeforeCollision:
        for (auto &component : components) {
            component->beforeCollision(data);
        }
        break;
    case EventType::DuringCollision:
        for (auto &component : components) {
            component->duringCollision(data);
        }
        break;
    case EventType::AfterCollision:
        for (auto &component : components) {
            component->afterCollision(data);
        }
        break;
    }
}

AABB GameObject::getTransformedAABB() {
    AABB* meshAabb = this->mesh->getAABB();
    aabb = multiplyAABBWithModelMatrix(meshAabb, m_modelMatrix);

    return aabb;
}

TypeIdentifier GameObject::getIdentifier() {
    return typeIdentifier;
}

void GameObject::setIdentifier(TypeIdentifier identifier) {
    this->typeIdentifier = identifier;
}

void GameObject::addCollidesWith(TypeIdentifier colliderID) {
    canCollideWith.push_back(colliderID);
}

void GameObject::addCollidesWith(initializer_list<TypeIdentifier> colliderIDs) {
    for (TypeIdentifier id : colliderIDs) {
        addCollidesWith(id);
    }
}
bool GameObject::collidesWith(TypeIdentifier id) {
    for (TypeIdentifier id2 : canCollideWith) {
        if (id2 == id) {
            return true;
        }
    }
    return false;
}
void GameObject::clearCollidesWithList() {
    canCollideWith = vector<TypeIdentifier>();
}

bool GameObject::isDynamicObject() {
    return dynamicObject;
}

void GameObject::setDynamic(bool isDynamic) {
    dynamicObject = isDynamic;
}

Octree* GameObject::getOctree() {
    return octree;
}

int GameObject::getId() {
    return id;
}

float3 GameObject::getScale() {
    return scale;
}

Quaternion GameObject::getRotation() {
    return rotation;
}

float3 GameObject::getLocation() {
    return location;
}

void GameObject::updateRotation(Quaternion r) {
    setRotation(hasRotation ? r*getRotation() : r);
}

void GameObject::setScale(float3 s) {
    scale = s;
    changed = true;
}

void GameObject::setLocation(float3 l) {
    location = l;
    changed = true;
}

void GameObject::setRotation(Quaternion r) {
    rotation = r;
    hasRotation = changed = true;
}

void GameObject::addChild(std::pair<GameObject*, float4x4> child) {
	children.push_back(child);
}

