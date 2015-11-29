#ifndef BUBBA_3D_RENDERCOMPONENT_H
#define BUBBA_3D_RENDERCOMPONENT_H

#include "IComponent.h"

class IRenderComponent : public IComponent {
public:
    virtual void render() = 0;
    virtual void renderShadow(Shader *shaderProgram) = 0;
private:

};


#endif //BUBBA_3D_RENDERCOMPONENT_H
