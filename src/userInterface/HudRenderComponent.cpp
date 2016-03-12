//
// Created by simon on 2016-03-12.
//

#include <float4x4.h>
#include <Shader.h>
#include "HudRenderComponent.h"

HudRenderComponent::HudRenderComponent(Shader* shaderProgram, float4x4 orthographicProjection) {
    this->shaderProgram = shaderProgram;
    this->orthographicProjection = orthographicProjection;
}

void HudRenderComponent::render() {

}