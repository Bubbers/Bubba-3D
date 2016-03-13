#ifndef BUBBA_3D_STANDARDRENDERER_H
#define BUBBA_3D_STANDARDRENDERER_H


#include "IRenderComponent.h"


class Mesh;
class ShaderProgram;
class GameObject;

/**
 * \brief Renders meshes with the default shader
 *
 * Class is responsible for rendering a Mesh/GameObject using
 * the default shader simple.vert and simple.frag. 
 *
 */
class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(Mesh*, GameObject*, ShaderProgram*);

    void update(float dt);

    void render();
    void renderShadow(ShaderProgram *shaderProgram);
private:
    Mesh* mesh;
    GameObject *gameObject;
};


#endif //BUBBA_3D_STANDARDRENDERER_H
