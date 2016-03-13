//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H

#include <string>
#include <IShader.h>
#include <GL/glew.h>

#define VERTEX_SHADER_NAME_STRING "Vertex_Shader"

/**
 * \brief Compiles Vertex Shaders 
 *
 * Class that takes a vertex shader string and compiles it.
 * It also checks for any possible errors that occured during compilation.
 *
 */
class VertexShader : public IShader {
public:
    VertexShader() {};
    VertexShader(std::string shaderName);

    virtual void compile();
    virtual void checkErrors();
    virtual GLuint getGLId();

private:
    std::string shaderString;
    GLuint vertexShader;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_VERTEXSHADER_H
