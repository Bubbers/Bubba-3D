//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
#define SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H

#include <GL/glew.h>
#include <float4x4.h>

using namespace chag;

class Shader;
class HUDGraphic;

class GLSquare {
public:
    virtual void render(Shader* shaderProgram, float4x4* projectionMatrix);
    GLSquare(float posX, float posY, float width, float height, HUDGraphic* image);
    virtual ~GLSquare();

    virtual void setRelativePosition(float3 position);
    virtual void setCenterOffset(float3 offset);
    virtual void setRotation(float rotation);

protected:
    float posX,posY,width,height, rotation = 0.0f;
    HUDGraphic* graphic;
    GLuint vao;
    float3 originalPosition,relativePosition,center;

    void updateOriginalPosition();
    void init(float posX, float posY, float width, float height, HUDGraphic* image);
    void fillVertexBuffer();
    void bindTextureAndDraw(Shader* shaderProgram, float4x4* projectionMatrix);
    float4x4 getModelMatrix();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
