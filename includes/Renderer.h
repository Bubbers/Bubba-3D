#ifndef __RENDERER_H__
#define __RENDERER_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "Utils.h"
#include "Effects.h"
#include <SFML/Window.hpp>

#define CUBE_MAP_RESOLUTION		512
#define SHADOW_MAP_RESOLUTION	2048

using namespace chag;

class Camera;
class Scene;
class IDrawable;

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void initGL();

	void drawScene(Camera *camera, Scene* scene, float currentTime);
	void start(unsigned int maxFPS);

	/**
	 * The idle function is called between each frame. Both parameters
	 * are time specified in seconds. The first parameter is the total
	 * time since the call to Renderer::start(int maxFPS). The second
	 * parameter is the time elapsed since the last idle method call.
	 *
	 * This function should contain all the game logic.
	 */
	void setIdleMethod(void(*idle)(float sinceStart,float sinceLastMethodCall));

	/**
	 * The display function is called when a frame should render. Both parameters
	 * are time specified in seconds. The first parameter is the total
	 * time since the call to Renderer::start(int maxFPS). The second
	 * parameter is the time elapsed since the idle method call for the previous frame.
	 *
	 * In this function Renderer::drawScene() should be called.
	 */
	void setDisplayMethod(void(*display)(float sinceStart,float sinceLastMethodCall));

	void swapBuffer();
	sf::Window* getWindow();

	void resize(unsigned int width, unsigned int height);

	Effects effects;
private:
	float currentTime;
	sf::Window* window;
	void(*idleMethod)(float,float);
	void(*displayMethod)(float,float);

	Fbo createPostProcessFbo(int width, int height);
	void drawShadowMap(Fbo sbo, float4x4 viewProjectionMatrix, Scene *scene);
	void drawShadowCasters(Shader* shaderProgram, Scene *scene);
	void drawTransparent(Shader* shaderProgram, Scene *scene);
	void setFog(Shader* shaderProgram);
	void setLights(Shader* shaderProgram, Scene *scene);

	Shader* shaderProgram;

	Fbo sbo;
	Camera *cubeMapCameras[6];

	//Drawing
	void drawModel(IDrawable &model, Shader* shaderProgram);
	void drawFullScreenQuad();
	void renderPostProcess();
	void blurImage();


	//Postprocess
	Shader* postFxShader;
	Shader* horizontalBlurShader;
	Shader* verticalBlurShader;
	Shader* cutoffShader;
	Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;
};


#endif // __RENDERER_H__
