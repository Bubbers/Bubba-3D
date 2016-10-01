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
#ifndef __RENDERER_H__
#define __RENDERER_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "linmath/float4x4.h"
#include "Utils.h"
#include "Effects.h"

#define CUBE_MAP_RESOLUTION	   512
#define SHADOW_MAP_RESOLUTION  2048


class Camera;
class Scene;
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
    void drawShadowCasters(ShaderProgram* shaderProgram, Scene *scene);
    void drawTransparent(ShaderProgram* shaderProgram, Scene *scene);
    void setFog(ShaderProgram* shaderProgram);
    void setLights(ShaderProgram* shaderProgram, Scene *scene);

    ShaderProgram* shaderProgram;

    Fbo sbo;
    Camera *cubeMapCameras[6];

    // Drawing
    void drawModel(IDrawable &model, ShaderProgram* shaderProgram);
    void drawFullScreenQuad();
    void renderPostProcess();
    void blurImage();

    // Postprocess
    ShaderProgram* postFxShader;
    ShaderProgram* horizontalBlurShader;
    ShaderProgram* verticalBlurShader;
    ShaderProgram* cutoffShader;
    Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;
};


#endif // __RENDERER_H__
