//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_HUDRENDERER_H
#define BUBBA_3D_HUDRENDERER_H

#include "IRenderComponent.h"
#include "GL/glew.h"

class HudRenderer : public IRenderComponent
{
public:
    HudRenderer();
    ~HudRenderer();

    virtual void render();
    virtual void renderShadow(Shader *shaderProgram);
    virtual void update(float dt);
    void render2DHud(Texture* texture, float4x4 *modelMatrix);

private:
    GLuint m_vaob;

};

#endif //BUBBA_3D_HUDRENDERER_H
