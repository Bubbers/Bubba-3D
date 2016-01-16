#ifndef BUBBA_3D_RENDERCOMPONENT_H
#define BUBBA_3D_RENDERCOMPONENT_H

#include "IComponent.h"

class Shader;

class IRenderComponent : public IComponent {
public:
    virtual void render() = 0;
    virtual void renderShadow(Shader *shaderProgram) = 0;

protected:
    Shader *shaderProgram;

};


#endif //BUBBA_3D_RENDERCOMPONENT_H
