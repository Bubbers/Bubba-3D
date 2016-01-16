#ifndef BUBBA_3D_STANDARDRENDERER_H
#define BUBBA_3D_STANDARDRENDERER_H


#include "IRenderComponent.h"


class Mesh;
class Shader;
class GameObject;

class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(Mesh*, GameObject*, Shader*);

    void update(float dt);
    void render();
    void renderShadow(Shader *shaderProgram);
private:
    Mesh* mesh;
    GameObject *gameObject;
};


#endif //BUBBA_3D_STANDARDRENDERER_H
