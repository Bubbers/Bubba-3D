/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
/*
* This file is part of Bubba-3D.
*
* Bubba-3D is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Bubba-3D is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
*/

#include <algorithm>
#include "linmath/Quaternion.h"
#include "Sphere.h"
#include "GameObject.h"
#include "linmath/float3x3.h"
#include "Collider.h"
#include "Mesh.h"
#include "IComponent.h"
#include "IRenderComponent.h"
#include "Octree.h"
#include "ShaderProgram.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

int GameObject::uniqueId = 0;

GameObject::GameObject() {
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    id = getUniqueId();
}

GameObject::GameObject(std::shared_ptr<GameObject> parent) {
    this->parent = parent;
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    id = getUniqueId();
}

GameObject::GameObject(std::shared_ptr<IMesh> mesh) {
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    initGameObject(mesh, mesh);
}

GameObject::GameObject(std::shared_ptr<IMesh> mesh, std::shared_ptr<IMesh> colliderMesh) {
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    initGameObject(mesh, colliderMesh);
}

GameObject::GameObject(std::shared_ptr<IMesh> mesh, std::shared_ptr<GameObject> parent) {
    this->parent = parent;
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    initGameObject(mesh, mesh);
}

GameObject::GameObject(std::shared_ptr<IMesh> mesh, std::shared_ptr<IMesh> colliderMesh, std::shared_ptr<GameObject> parent) {
    this->parent = parent;
    m_modelMatrix = chag::make_identity<chag::float4x4>();
    initGameObject(mesh, colliderMesh);
}

void GameObject::initGameObject(std::shared_ptr<IMesh> &mesh, std::shared_ptr<IMesh> &colliderMesh) {
    this->mesh = mesh;
    this->collisionMesh = colliderMesh;
    this->m_modelMatrix = chag::make_identity<chag::float4x4>();
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

Octree* GameObject::createOctree(std::shared_ptr<IMesh> &mesh) {
    AABB* aabb = mesh->getAABB();
    chag::float3 halfVector = (aabb->maxV - aabb->minV) / 2;
    chag::float3 origin = aabb->maxV - halfVector;
    Octree* octree = new Octree(origin, halfVector);

    std::vector<Triangle *> triangles = mesh->getTriangles();
    octree->insertAll(triangles);

    return octree;
}

Sphere GameObject::getTransformedSphere() {
    chag::float3 absScale = getAbsoluteScale();
    float scaling = std::max(absScale.x, std::max(absScale.y, absScale.z));
    return Sphere(sphere.getPosition()+getAbsoluteLocation(), scaling*sphere.getRadius());
}

void GameObject::makeDirty() {
    for (IComponent *component : components) {
        component->onDeath();
    }
    dirty = true;
}

bool GameObject::isDirty() {
    return dirty;
}

void GameObject::move(chag::float4x4 modelMatrix) {
    m_modelMatrix = modelMatrix;
}

void GameObject::update(chag::float4x4 updateMatrix) {
    m_modelMatrix = m_modelMatrix * updateMatrix;
}

void GameObject::render() {
    if (renderComponent != nullptr) {
        renderComponent->render();
    }
    if (children.size() != 0) {
        for (auto child : children) {
            child->render();
        }
    }
}

std::vector<Triangle *> GameObject::getTriangles() {
    return mesh->getTriangles();
}

chag::float4x4 GameObject::getModelMatrix() {
    return m_modelMatrix;
}

void GameObject::renderEmissive(std::shared_ptr<ShaderProgram> &shaderProgram){
    if(renderComponent != nullptr){
        renderComponent->renderEmissive(shaderProgram);
    }
}

void GameObject::renderShadow(std::shared_ptr<ShaderProgram> &shaderProgram) {
    renderComponent->renderShadow(shaderProgram);
    for (auto child : children) {
        child->renderShadow(shaderProgram);
    }
}

void GameObject::addRenderComponent(IRenderComponent* renderer) {
    this->renderComponent = renderer;
    components.push_back(renderer);
    renderer->bind(shared_from_this());
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    newComponent->bind(shared_from_this());
}

chag::float4x4 GameObject::getFullMatrix() {
    chag::float4x4 parentMat;

    chag::float4x4 translation = chag::make_translation(this->getRelativeLocation());
    chag::float4x4 rotation    = hasRotation ? chag::makematrix(this->getRelativeRotation()) : chag::make_identity<chag::float4x4>();
    chag::float4x4 scale       = chag::make_scale<chag::float4x4>(this->getRelativeScale());

    parentMat = translation * rotation * scale;

    if(parent) {
        parentMat = parent->getFullMatrix() * parentMat;
    }

    return parentMat;
}


void GameObject::update(float dt) {
    for (IComponent *component : components) {
        component->update(dt);
    }
    if (changed) {
        changed = false;

        move(getFullMatrix());
    }
    for (auto child : children) {
        child->changed = true;
        child->update(dt);
    }
}

void GameObject::callEvent(EventType type, std::shared_ptr<GameObject> data) {
    switch (type) {
    case EventType::BeforeCollision:
        for (IComponent *component : components) {
            component->beforeCollision(data);
        }
        break;
    case EventType::DuringCollision:
        for (IComponent *component : components) {
            component->duringCollision(data);
        }
        break;
    case EventType::AfterCollision:
        for (IComponent *component : components) {
            component->afterCollision(data);
        }
        break;
    }
    for (auto child : children) {
        child->callEvent(type, data);
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

void GameObject::addCollidesWith(std::initializer_list<TypeIdentifier> colliderIDs) {
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
    canCollideWith = std::vector<TypeIdentifier>();
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

chag::float3 GameObject::getAbsoluteScale() {
    if (parent) {
        return scale * parent->getAbsoluteScale();
    }
    return scale;
}

chag::float3 GameObject::getRelativeScale() {
    return scale;
}

chag::Quaternion GameObject::getAbsoluteRotation() {
    if (parent) {
        return rotation * parent->getAbsoluteRotation();
    }
    return rotation;
}

chag::Quaternion GameObject::getRelativeRotation() {
    return rotation;
}

chag::float3 GameObject::getAbsoluteLocation() {
    chag::float3 pos;
    pos.x = m_modelMatrix.c4.x;
    pos.y = m_modelMatrix.c4.y;
    pos.z = m_modelMatrix.c4.z;
    return pos;
}

chag::float3 GameObject::getRelativeLocation() {
    return location;
}

void GameObject::updateRotation(chag::Quaternion r) {
    setRotation(hasRotation ? r * getRelativeRotation() : r);
}

void GameObject::setScale(chag::float3 s) {
    scale = s;
    changed = true;
}

void GameObject::setLocation(chag::float3 l) {
    location = l;
    changed = true;

}

void GameObject::setRotation(chag::Quaternion r) {
    rotation = r;
    hasRotation = changed = true;
}

void GameObject::addChild(std::shared_ptr<GameObject> child) {
    child->parent = shared_from_this();
    children.push_back(child);
}

void GameObject::initializeModelMatrix() {
    move(getFullMatrix());
}

std::vector<std::shared_ptr<GameObject>> *GameObject::getChildren() {
    return &children;
}
