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
#pragma once

#include "IDrawable.h"
#include "AABB2.h"
#include "linmath/float4x4.h"
#include <vector>
#include <linmath/Quaternion.h>
#include <Sphere.h>
#include <memory>

enum EventType {BeforeCollision, DuringCollision, AfterCollision};

typedef int TypeIdentifier;

class Mesh;
class Triangle;
class IRenderComponent;
class IComponent;
class Octree;
class ShaderProgram;

/**
 * \brief A class for containing all information about a object in the game world.
 *
 * Contains information about the mesh; how to render it, how to manipulate it and how to collide it.
 *
 * Standard usage:
 * \code
 *
 * //Initiation
 * GameObject object(mesh);
 * object.addRenderComponent(renderComp); //If the mesh should be rendered
 * object.addComponent(comp); //All other components, such as MoveComponents
 *
 * //Rendering
 * object.render();
 *
 * //Update all its internal components
 * object.update(timeSinceLastUpdate);
 *
 * \endcode
 *
 * If the object can move, call setDynamic(true) to indicate this.
 * The objects dynamicity can be used by collision handlers to determine
 * if two objects can possibly collide (eg, two static objects can never collide).
 *
 * The dirty flag can be used to indicate that an objects should be removed when possible.
 * This can be useful if calculations involving the object needs to be performed after it
 * has been marked as dirty. Then you can at a later point, when calculations arent being done,
 * safely remove the object.
 */
class GameObject : public std::enable_shared_from_this<GameObject>, public IDrawable {
public:

    /**
     * Creates a minimal instance of a GameObject.
     * This GameObject will not collide with anything, nor will there be any mesh to render.
     */
    GameObject();

    /**
     * Initiates the GameObject with the same mesh for rendering and collision
     *
     * @param mesh The mesh to render and perform collisions with
     */
    explicit GameObject(std::shared_ptr<Mesh> mesh);

    /**
     * Initiates the GameObject with different meshes for rendering and collisions
     *
     * @param mesh The mesh to use for rendering
     * @param colliderMesh The mesh to use for collision
     */
    GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> colliderMesh);

    virtual ~GameObject();

    /**
     * Renders the GameObject using the GameObjects RenderComponent, if any.
     */
    virtual void render();
    /**
     * Renders the GameObject shadows using the GameObjects RenderComponent, if any.
     */
    virtual void renderShadow(std::shared_ptr<ShaderProgram> &shaderProgram);

    /**
     * Add a IRenderComponent to the GameObject.
     */
    void addRenderComponent(IRenderComponent* renderer);

    /**
     * Add a component to the GameObject.
     */
    void addComponent(IComponent* newComponent);

    /**
     * Calls update for all components in the GameObject
     *
     * @param dt The time since last called
     */
    void update(float dt);

    /**
     * Calls a specific component event on all components in the GameObject
     *
     * @param type The event to call on the components
     * @param data // TODO(Bubbad) Do all events need to involve another gameobject?
     */
    void callEvent(EventType type, GameObject* data);

    /**
     * Get the model matrix of the GameObject.
     */
    chag::float4x4 getModelMatrix();

    /**
     * Replaces the old model matrix completely
     *
     * @param modelMatrix The new model matrix
     */
    void move(chag::float4x4 model_matrix);

    /**
     * Multiplies the old model matrix with the specified
     *
     * @param updateMatrix The matrix to multiply with
     */
    void update(chag::float4x4 update_matrix);

    /**
     * Get the scale of the GameObject relative to its parent.
     * If there is no parent, this is the same as getAbsoluteScale().
     */
    chag::float3 getRelativeScale();

    /**
     * Get scale of the GameObject relative to default scale.
     */
    chag::float3 getAbsoluteScale();

    /**
     * Set the absoulte scale of the GameObject.
     */
    void setScale(chag::float3 s);

    /**
     * Get the rotation of the GameObject relative to its parent.
     * If there is no parent, this is the same as getAbsoluteRotation().
     */
    chag::Quaternion getRelativeRotation();

    /**
     * Get the rotation of the GameObject relative to default rotation.
     */
    chag::Quaternion getAbsoluteRotation();

    /**
     * Set the absolute rotation of the GameObject.
     */
    void setRotation(chag::Quaternion r);

    /**
     * Rotate the GameObject.
     */
    void updateRotation(chag::Quaternion r);

    /**
     * Get the location of the GameObject relative to its parent.
     * If there is no parent, this is the same as getAbsoluteLocation().
     */
    chag::float3 getRelativeLocation();

    /**
     * Get the location of the GameObject relative to origo.
     */
    chag::float3 getAbsoluteLocation();

    /**
     * Sets the absolute location of the GameObject.
     */
    void setLocation(chag::float3 l);

    /**
     * Tries to make a parent child relationship between this GameObject and a
     * given child GameObject. Requires that the child being added dose not
     * already have a parent.
     *
     * @param child The GameObject to add as child.
     * @throws std::invalid_argument If the given child already has a parent.
     */
    void addChild(std::shared_ptr<GameObject> child);

    TypeIdentifier getIdentifier();
    void setIdentifier(TypeIdentifier identifier);

    void addCollidesWith(TypeIdentifier colliderID);
    void addCollidesWith(std::initializer_list<TypeIdentifier> colliderIDs);

    void clearCollidesWithList();
    bool collidesWith(TypeIdentifier id);

    /**
     * NOTE: The octree has not been transformed.
     *
     * @return The octree containing the GameObject
     */
    Octree* getOctree();

    /**
     * NOTE: The triangles have not been transformed.
     *
     * @return A list of all the triangles in the GameObject
     */
    std::vector<Triangle*> getTriangles();

    AABB getTransformedAABB();
    Sphere getTransformedSphere();

    bool isDynamicObject();
    void setDynamic(bool isDynamic);

    int getId();

    /**
     * Marks the GameObject as dirty, indicating that
     * the GameObject will be removed soon.
     *
     * CAUTION: THIS ACTION IS IRREVERSIBLE
     */
    void makeDirty();

    /**
     * Check if the GameObject is dirty or not.
     */
    bool isDirty();


private:
    void initGameObject(std::shared_ptr<Mesh> &mesh, std::shared_ptr<Mesh> &colliderMesh);

    chag::float4x4 getFullMatrix();

    /**
     * Returns a fresh id number.
     */
    int getUniqueId();

    /**
     * Creates an Octree from all the triangles of the specified mesh
     *
     * @param Mesh to build an octree from
     */
    Octree* createOctree(std::shared_ptr<Mesh> &mesh);

    static int uniqueId;
    int id;

    /* Mesh */
    std::shared_ptr<Mesh> mesh;
    chag::float4x4 m_modelMatrix;
    chag::float3 scale = chag::make_vector(1.0f, 1.0f, 1.0f);
    chag::Quaternion rotation;
    chag::Quaternion rotationPointReference;
    bool hasRotation = false;

    chag::float3 location = chag::make_vector(0.0f, 0.0f, 0.0f);

    bool changed = false;

    /* Hierarchy */
    std::shared_ptr<GameObject> parent;
    std::vector<std::shared_ptr<GameObject>> children;

    /* Collision */
    std::shared_ptr<Mesh> collisionMesh;
    AABB aabb;
    Sphere sphere;
    Octree *octree;
    TypeIdentifier typeIdentifier = -1;
    std::vector<TypeIdentifier> canCollideWith;
    bool dynamicObject = false;

    /* Components */
    IRenderComponent* renderComponent = nullptr;
    std::vector<IComponent*> components;

    bool dirty = false;
};
