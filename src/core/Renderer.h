#ifndef __RENDERER_H__
#define __RENDERER_H__

#ifdef WIN32
#include <windows.h>
#endif


#include <GL/glew.h>
#include <GL/freeglut.h>

#include <IL/il.h>
#include <IL/ilut.h>

#include <glutil\glutil.h>
#include <float4x4.h>
#include <float3x3.h>

#include "Logger.h"
#include "PerspectiveCamera.h"
#include "..\objects\Scene.h"
#include <Octree.h>

#define CUBE_MAP_RESOLUTION		512
#define SHADOW_MAP_RESOLUTION	2048

using namespace chag;

//*****************************************************************************
//	Frame buffer objects
//*****************************************************************************
struct Fbo{
	GLuint id;
	GLuint texture;
	GLuint shaderProgram;
	GLuint depthbuffer;

	int width, height;
};

class Renderer
{
public:
	Renderer(int argc, char *argv[], int width, int height);
	~Renderer();

	void initGL(float3 carLoc);

	void drawScene(Camera camera, Scene scene, float currentTime);
	void start();
	void render();

	void setIdleMethod(void(*idle)(int), float delay);
	void setDisplayMethod(void (*display)(void));

	void swapBuffer() {
		glutSwapBuffers();  // swap front and back buffer. This frame will now be displayed.
		CHECK_GL_ERROR();
	}

private:
	int width, height;
	float currentTime;

	

	Fbo createPostProcessFbo(int width, int height);
	void drawShadowMap(Fbo sbo, float4x4 viewProjectionMatrix, Scene scene);
	void drawShadowCasters(GLuint shaderProgram, Scene scene);
	void setFog(GLuint shaderProgram);

	GLuint shaderProgram;
	Logger logger = Logger::instance();

	Fbo sbo;
	Fbo cMapAll;
	Camera *cubeMapCameras[6];
	CubeMapTexture* reflectionCubeMap;
	
	//Drawing
	void drawModel(Mesh &model, GLuint shaderProgram);
	void drawFullScreenQuad();
	void renderPostProcess();
	void blurImage();


	//Postprocess
	GLuint postFxShader;
	GLuint horizontalBlurShader;
	GLuint verticalBlurShader;
	GLuint cutoffShader;
	Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;

	//void renderPostProcess();
	//void blurImage();

	//DEBUGS
	void drawDebug(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, Scene scene);
	void debugDrawLine(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 rayVector);
	void debugDrawQuad(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 halfVector);
	void debugDrawOctree(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, Octree tree);
};


#endif // __RENDERER_H__