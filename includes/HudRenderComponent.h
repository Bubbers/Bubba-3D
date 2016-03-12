//
// Created by simon on 2016-03-12.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_HUDRENDERCOMPONENT_H
#define SUPER_BUBBA_AWESOME_SPACE_HUDRENDERCOMPONENT_H

#include <IRenderComponent.h>

using namespace chag;

class HudRenderComponent  : public IRenderComponent {
public:
    virtual void render();
    void renderShadow(Shader *shaderProgram){}
    HudRenderComponent(Shader* shaderProgram, float4x4 orthographicProjection);

private:
    float4x4 orthographicProjection;
};


#endif //SUPER_BUBBA_AWESOME_SPACE_HUDRENDERCOMPONENT_H
