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
