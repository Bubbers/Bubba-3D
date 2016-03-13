#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include "IDrawable.h"
#include "AABB2.h"
#include "linmath/float4x4.h"
#include <vector>
#include <linmath/Quaternion.h>
#include <Sphere.h>
#include "ShaderProgram.h"

using namespace chag;
using namespace std;

enum EventType {BeforeCollision, DuringCollision, AfterCollision};

typedef int TypeIdentifier;

class Mesh;
class Triangle;
class IRenderComponent;
class IComponent;
class Octree;

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
class GameObject : public IDrawable {
public:
    GameObject();
	GameObject(GameObject* parent);

    /**
     * Initiates the GameObject with the same mesh for rendering and collision
     *
     * @param mesh The mesh to render and perform collisions with
     */
    explicit GameObject(Mesh *mesh);
    /**
     * Initiates the GameObject with different meshes for rendering and collisions
     *
     * @param mesh The mesh to use for rendering
     * @param colliderMesh The mesh to use for collision
     */
    GameObject(Mesh *mesh, Mesh *colliderMesh);

	GameObject(Mesh *mesh, GameObject* parent);
	GameObject(Mesh *mesh, Mesh *colliderMesh, GameObject* parent);

    virtual ~GameObject();

    /**
     * Renders the GameObject using the GameObjects RenderComponent, if any.
     */
    virtual void render();
    /**
     * Renders the GameObject shadows using the GameObjects RenderComponent, if any.
     */
    virtual void renderShadow(ShaderProgram* shaderProgram);

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
    void callEvent(EventType type, GameObject* data);


    float4x4 getModelMatrix();
    /**
     * Replaces the old model matrix completely
     *
     * @param modelMatrix The new model matrix
     */
    void move(float4x4 model_matrix);
    /**
     * Multiplies the old model matrix with the specified
     *
     * @param updateMatrix The matrix to multiply with
     */
    void update(float4x4 update_matrix);

    float3 getRelativeScale();
	float3 getAbsoluteScale();
	void setScale(float3 s);

    Quaternion getRelativeRotation();
	Quaternion getAbsoluteRotation();
    void setRotation(Quaternion r);
    void updateRotation(Quaternion r);

    float3 getRelativeLocation();
	float3 getAbsoluteLocation();
    void setLocation(float3 l);

	void addChild(GameObject* child);

    TypeIdentifier getIdentifier();
    void setIdentifier(TypeIdentifier identifier);
    void addCollidesWith(TypeIdentifier colliderID);
    void addCollidesWith(initializer_list<TypeIdentifier> colliderIDs);
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


private:
    void initGameObject(Mesh *mesh, Mesh *colliderMesh);

    /**
     * Returns a fresh id number.
     */
    int getUniqueId();

    /**
     * Creates an Octree from all the triangles of the specified mesh
     *
     * @param Mesh to build an octree from
     */
    Octree* createOctree(Mesh* mesh);

    static int uniqueId;
    int id;

    /* Mesh */
    Mesh *mesh;
    chag::float4x4 m_modelMatrix;
    float3 scale = make_vector(1.0f, 1.0f, 1.0f);
    Quaternion rotation = Quaternion();
    Quaternion rotationPointReference = Quaternion();
    bool hasRotation = false;
    float3 location = make_vector(0.0f, 0.0f, 0.0f);
    bool changed = false;

	/* Hierarchy */
	GameObject* parent = nullptr;
	std::vector<GameObject*> children;

    /* Collision */
    Mesh *collisionMesh;
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


#endif
