//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_HUDRENDERER_H
#define BUBBA_3D_HUDRENDERER_H

#include "IRenderComponent.h"
#include "GL/glew.h"
#include "Utils.h"
#include <map>

using namespace std;

class Texture;
class Layout;
class GLSquare;

class HudRenderer : public IRenderComponent
{
public:
    HudRenderer();
    ~HudRenderer();

    virtual void render();
    virtual void renderShadow(ShaderProgram *shaderProgram);
    virtual void update(float dt);

    virtual void setLayout(Layout* layout);

    virtual GLSquare* getSquareByID(string id);

protected:
    float4x4 createOrthographicProjection();
    map<string,GLSquare*> squares;

};

#endif //BUBBA_3D_HUDRENDERER_H
