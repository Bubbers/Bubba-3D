//
// Created by johan on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H
#define SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H

#include <IShader.h>

/**
* \brief Compiles Fragment Shaders
*
* Class that takes a fragment shader string and compiles it.
* It also checks for any possible errors that occured during compilation.
*
*/
class FragmentShader : public IShader{
public:
    FragmentShader() {};
    FragmentShader(std::string shaderName);

    virtual void compile();
    virtual void checkErrors();
    virtual GLuint getGLId();

private:
    std::string shaderString;
    GLuint fragmentShader;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_FRAGMENTSHADER_H
