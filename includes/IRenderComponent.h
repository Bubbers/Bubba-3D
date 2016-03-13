#ifndef BUBBA_3D_RENDERCOMPONENT_H
#define BUBBA_3D_RENDERCOMPONENT_H

#include "IComponent.h"
#include "ShaderProgram.h"


class IRenderComponent : public IComponent {
public:
    virtual void render() = 0;
    virtual void renderShadow(ShaderProgram *shaderProgram) = 0;

protected:
    ShaderProgram *shaderProgram;

};


#endif //BUBBA_3D_RENDERCOMPONENT_H
