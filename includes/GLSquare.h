//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
#define SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H

#include <GL/glew.h>
#include <linmath/float4x4.h>
#include <IHudDrawable.h>

using namespace chag;

class ShaderProgram;
class HUDGraphic;

/**
 * Represents a low level gl square with a HUDGraphic as its
 * graphical element
 */
class GLSquare : public IHudDrawable {
public:
    virtual void render(ShaderProgram* shaderProgram, float4x4* projectionMatrix);
    GLSquare(float posX, float posY, float width, float height, HUDGraphic* image);
    virtual ~GLSquare();

    void setCenterOffset(float3 offset);
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    HUDGraphic* getGraphic();


protected:
    float posX,posY,width,height;
    HUDGraphic* graphic;
    GLuint vao;
    ShaderProgram* shaderProgram;
    float4x4* projectionMatrix;
    float3 originalPosition;

    void init(float posX, float posY, float width, float height, HUDGraphic* image);
    void fillVertexBuffer();
    void bindTextureAndDraw(ShaderProgram* shaderProgram, float4x4* projectionMatrix);
    float4x4 getModelMatrix();
    void updateOriginalPosition();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
