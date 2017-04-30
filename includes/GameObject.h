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
class GameObject : public IDrawable, public std::enable_shared_from_this<GameObject> {
public:
    GameObject();
    GameObject(std::shared_ptr<GameObject> parent);

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

    GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<GameObject> parent);
    GameObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Mesh> colliderMesh, std::shared_ptr<GameObject> parent);

    virtual ~GameObject();

    /**
     * Renders the GameObject using the GameObjects RenderComponent, if any.
     */
    virtual void render();
    /**
     * Renders the GameObject shadows using the GameObjects RenderComponent, if any.
     */
    virtual void renderShadow(std::shared_ptr<ShaderProgram> &shaderProgram);

    void renderEmissive(std::shared_ptr<ShaderProgram> &shaderProgram);

    void addRenderComponent(IRenderComponent* renderer);
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
    void callEvent(EventType type, std::shared_ptr<GameObject> data);


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

    chag::float3 getRelativeScale();
    chag::float3 getAbsoluteScale();
    void setScale(chag::float3 s);

    chag::Quaternion getRelativeRotation();
    chag::Quaternion getAbsoluteRotation();
    void setRotation(chag::Quaternion r);
    void updateRotation(chag::Quaternion r);

    chag::float3 getRelativeLocation();
    chag::float3 getAbsoluteLocation();
    void setLocation(chag::float3 l);

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
     * it should be removed at some point.
     */
    void makeDirty();
    bool isDirty();

    void initializeModelMatrix();


    std::vector<std::shared_ptr<GameObject>> *getChildren();

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
    std::shared_ptr<GameObject> parent = nullptr;
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
