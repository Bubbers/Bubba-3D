//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_HUDRENDERER_H
#define BUBBA_3D_HUDRENDERER_H

#include "IRenderComponent.h"
#include "GL/glew.h"
#include "Utils.h"

class Texture;

class HudRenderer : public IRenderComponent
{
public:
    HudRenderer(int *scoreBoard, State* state);
    ~HudRenderer();
    struct HudConfig{
        //percentage (0-100)
        float speed = 0.0f;
    };

    virtual void render();
    virtual void renderShadow(ShaderProgram *shaderProgram);
    virtual void update(float dt);
    void render2DHud(Texture* texture, float4x4 *modelMatrix);
    struct HudConfig* getConfig();

private:
    void renderNum(int, float4x4 *modelMatrix);

    bool start = true;

    int *scoreBoard;
    GLuint m_vaob;
    struct HudConfig* conf;
    State *state;

};

#endif //BUBBA_3D_HUDRENDERER_H
