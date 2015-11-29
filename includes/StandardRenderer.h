#ifndef BUBBA_3D_STANDARDRENDERER_H
#define BUBBA_3D_STANDARDRENDERER_H

#include "Mesh.h"
#include "float4x4.h"
#include "Shader.h"
#include "IRenderComponent.h"

using namespace chag;

class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(Mesh*, float4x4*, Shader*);

    void update(float dt);
    void render();
    void renderShadow(Shader *shaderProgram);
private:
    Mesh* mesh;
    float4x4* modelMatrix;
    Shader* shaderProgram;
};


#endif //BUBBA_3D_STANDARDRENDERER_H
