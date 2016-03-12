//
// Created by simon on 2016-03-12.
//

#include <linmath/float4x4.h>
#include <ShaderProgram.h>
#include "HudRenderComponent.h"

HudRenderComponent::HudRenderComponent(ShaderProgram* shaderProgram, float4x4 orthographicProjection) {
    this->shaderProgram = shaderProgram;
    this->orthographicProjection = orthographicProjection;
}

void HudRenderComponent::render() {

}