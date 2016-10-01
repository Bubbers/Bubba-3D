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

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

int GameObject::uniqueId = 0;

GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
    id = getUniqueId();
}

GameObject::GameObject(GameObject* parent) {
	this->parent = parent;
	m_modelMatrix = make_identity<float4x4>();
	id = getUniqueId();
}

GameObject::GameObject(Mesh *mesh) {
    m_modelMatrix = make_identity<float4x4>();
    initGameObject(mesh, mesh);
}

GameObject::GameObject(Mesh *mesh, Mesh *colliderMesh) {
    m_modelMatrix = make_identity<float4x4>();
    initGameObject(mesh, colliderMesh);
}

GameObject::GameObject(Mesh *mesh, GameObject* parent) {
	this->parent = parent;
	m_modelMatrix = make_identity<float4x4>();
	initGameObject(mesh, mesh);
}

GameObject::GameObject(Mesh *mesh, Mesh *colliderMesh, GameObject* parent) {
	this->parent = parent;
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
    AABB* aabb = mesh->getAABB();
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
			child->render();
		}
	}
}

std::vector<Triangle *> GameObject::getTriangles() {
    return mesh->getTriangles();
}

float4x4 GameObject::getModelMatrix() {
    return m_modelMatrix;
}


void GameObject::renderShadow(ShaderProgram *shaderProgram) {
    renderComponent->renderShadow(shaderProgram);
	for (auto child : children) {
		child->renderShadow(shaderProgram);
	}
}

void GameObject::addRenderComponent(IRenderComponent* renderer) {
    this->renderComponent = renderer;
    components.push_back(renderer);
    renderer->bind(this);
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    newComponent->bind(this);
}

float4x4 GameObject::getFullMatrix() {
    float4x4 parentMat;

    float4x4 translation = make_translation(this->getRelativeLocation());
    float4x4 rotation    = makematrix(this->getRelativeRotation());
    float4x4 scale = make_scale<float4x4>(this->getRelativeScale());

    parentMat = translation * rotation * scale;

    if(parent != nullptr) {
        parentMat = parent->getFullMatrix() * parentMat;
    }

    return parentMat;
}


void GameObject::update(float dt) {
    for (auto &component : components) {
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

float3 GameObject::getAbsoluteScale() {
	if (parent != nullptr) {
		return scale * parent->getAbsoluteScale();
	}
	return scale;
}

float3 GameObject::getRelativeScale() {
	return scale;
}

Quaternion GameObject::getAbsoluteRotation() {
	if (parent != nullptr) {
		return rotation * parent->getAbsoluteRotation();
	}
	return rotation;
}

Quaternion GameObject::getRelativeRotation() {
	return rotation;
}

float3 GameObject::getAbsoluteLocation() {
	float3 pos;
	pos.x = m_modelMatrix.c4.x;
	pos.y = m_modelMatrix.c4.y;
	pos.z = m_modelMatrix.c4.z;
	return pos;
}

float3 GameObject::getRelativeLocation() {
	return location;
}

void GameObject::updateRotation(Quaternion r) {
    setRotation(hasRotation ? r*getRelativeRotation() : r);
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

void GameObject::addChild(GameObject* child) {
	children.push_back(child);
}

