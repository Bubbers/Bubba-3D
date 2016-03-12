//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
#define SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H

#include <GL/glew.h>
#include <float4x4.h>
#include <IHudDrawable.h>

using namespace chag;

class Shader;
class HUDGraphic;

class GLSquare : public IHudDrawable {
public:
    virtual void render(Shader* shaderProgram, float4x4* projectionMatrix);
    GLSquare(float posX, float posY, float width, float height, HUDGraphic* image);
    virtual ~GLSquare();

    void setCenterOffset(float3 offset);

protected:
    float posX,posY,width,height;
    HUDGraphic* graphic;
    GLuint vao;
    Shader* shaderProgram;
    float4x4* projectionMatrix;
    float3 originalPosition;

    void init(float posX, float posY, float width, float height, HUDGraphic* image);
    void fillVertexBuffer();
    void bindTextureAndDraw(Shader* shaderProgram, float4x4* projectionMatrix);
    float4x4 getModelMatrix();
    void updateOriginalPosition();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
