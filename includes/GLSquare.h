//
// Created by simon on 2016-02-02.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
#define SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H

class Texture;

class GLSquare {
public:
    void render(Shader* shaderProgram, GLuint* vertexBuffer);
    GLSquare(float posX, float posY, float width, float height, Texture* image);

    static void initVertexBufferForGLSquare(GLuint* vertexBuffer);

private:
    float posX,posY,width,height;
    Texture* texture;

    void fillVertexBuffer(GLuint* vertexBuffer);
    void bindTextureAndDraw(Shader* shaderProgram, GLuint* vertexBuffer);
    float4x4 getModelMatrix();

};


#endif //SUPER_BUBBA_AWESOME_SPACE_GLSQUARE_H
