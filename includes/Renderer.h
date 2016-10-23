/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include "linmath/float4x4.h"
#include "Effects.h"
#include "Utils.h"
#include <memory>

#define CUBE_MAP_RESOLUTION	   512
#define SHADOW_MAP_RESOLUTION  2048

class Camera;
class Scene;
class ShaderProgram;
class IDrawable;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initGL();

    void drawScene(Camera *camera, Scene* scene, float currentTime);

    void swapBuffer();

    void resize(unsigned int width, unsigned int height);

    void initRenderer(int width, int height);

    Effects effects;
private:
    float currentTime;

    Fbo createPostProcessFbo(int width, int height);
    void drawShadowMap(Fbo sbo, chag::float4x4 viewProjectionMatrix, Scene *scene);
    void drawShadowCasters(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene);
    void drawTransparent(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene);
    void setFog(std::shared_ptr<ShaderProgram> &shaderProgram);
    void setLights(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene);
    void drawBloom(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene, int i, int i1, chag::float4x4 &viewProjectionMatrix);

    std::shared_ptr<ShaderProgram> shaderProgram;

    Fbo sbo;
    Camera *cubeMapCameras[6];


    // Drawing
    void drawModel(IDrawable &model, std::shared_ptr<ShaderProgram> &shaderProgram);
    void drawFullScreenQuad();
    void renderPostProcess();
    void blurImage();

    // Postprocess
    std::shared_ptr<ShaderProgram> postFxShader;
    std::shared_ptr<ShaderProgram> horizontalBlurShader;
    std::shared_ptr<ShaderProgram> verticalBlurShader;
    std::shared_ptr<ShaderProgram> cutoffShader;
    std::shared_ptr<ShaderProgram> emissiveShader;
    Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;

};
