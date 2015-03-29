#ifdef WIN32
#include <windows.h>
#endif


#include <GL/glew.h>
#include <GL/freeglut.h>

#include <IL/il.h>
#include <IL/ilut.h>

#include <stdlib.h>
#include <algorithm>

#include <glutil\glutil.h>
#include <float4x4.h>
#include <float3x3.h>
#include "AABB.h"
#include <Quaternion.h>
#include "collision\Collider.h"

#include <vector>
#include <thread>

#include <ctime>

#include "Octree.h"
#include "Triangle.h"
#include "SceneRenderer.h"
#include "PerspectiveCamera.h"
#include "Logger.h"
#include <chrono>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "OBJModel.h"
#include "CubeMapTexture.h"
#include "Skybox.h"

using namespace std;
using namespace chag;
using namespace chrono;


#define FOG_EQUATION_LINEAR 0
#define FOG_EQUATION_EXP    1
#define FOG_EQUATION_EXP2   2
#define FOG_EQUATION_NONE   3

#define SCREEN_WIDTH			1028
#define SCREEN_HEIGHT			800

#define SHADOW_MAP_RESOLUTION	2048
#define CUBE_MAP_RESOLUTION		512

#define TICK_PER_SECOND  50


namespace FogParams {
	float fDensity = 0.001f;
	float fStart = 50.0f;
	float fEnd = 5000.0f;
	float3 vColor = make_vector(1.0f, 1.0f, 1.0f);
	int fEquation = FOG_EQUATION_LINEAR;
};

//*****************************************************************************
//	Global variables
//*****************************************************************************
bool paused = false;				// Tells us wether sun animation is paused
float currentTime = 0.0f;			// Tells us the current time
float timeSinceDraw = 0.0f;
GLuint shaderProgram;
const float3 vUp = make_vector(0.0f, 1.0f, 0.0f);

Octree *octTree; //(make_vector(0.0f, 0.0f, 0.0f), make_vector(200.0f, 50.0f, 200.0f), 0);


GLuint postFxShader;
GLuint horizontalBlurShader;
GLuint verticalBlurShader;
GLuint cutoffShader;

//*****************************************************************************
//	OBJ Model declarations
//*****************************************************************************
Mesh world;
Mesh worldCollision;
Mesh car;
Mesh factory;
Mesh water;
Mesh spider;

//*****************************************************************************
//	Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;
Skybox* skybox;


//*****************************************************************************
//	Camera state variables (updated in motion())
//*****************************************************************************
float camera_theta = M_PI / 1.0f;
float camera_phi = M_PI / 4.0f;
float camera_r = 30.0; 
float camera_target_altitude = 5.2; 

//*****************************************************************************
//	Light state variables (updated in idle())
//*****************************************************************************
float3 lightPosition = {30.1f, 450.0f, 0.1f};

//****************************************************************************
//	Input state variables
//*****************************************************************************
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;
int prev_x = 0;
int prev_y = 0;
bool keysDown[256];

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

Fbo sbo;
Fbo cMapAll;
Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;

//*****************************************************************************
//	Camera
//*****************************************************************************

Camera *cubeMapCameras[6];
Camera *sunCamera;
Camera *playerCamera;
int camera = 6;

//*****************************************************************************
//	Car object variables
//*****************************************************************************

struct Car{
	float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);
	float3 upDir = make_vector(0.0f, 1.0f, 0.0f);
	float3 location = make_vector(0.0f, 0.0f, 0.0f);
	float3  
		wheel1 = make_vector( 1.2f, 0.0f,  1.5f),
		wheel2 = make_vector( 1.2f, 0.0f, -1.5f),
		wheel3 = make_vector(-0.8f, 0.0f,  1.5f),
		wheel4 = make_vector(-0.8f, 0.0f, -1.5f);

	float rotationSpeed = 2 * M_PI / 180;
	float moveSpeed = 0.5;
	float angley = 0, anglez, anglex;
	float lengthx = 2, lengthz = 3;
} carLoc;

//*****************************************************************************
//	Collision objects
//*****************************************************************************
Collider *collider;
bool hasChanged = true;

//***********s******************************************************************
//	Logger
//*****************************************************************************
Logger logger;


//*****************************************************************************
//	Function declarations
//*****************************************************************************

void drawCubeMap(Fbo fbo);
void drawFullScreenQuad();

Fbo createPostProcessFbo(int width, int height);
void renderPostProcess();
void blurImage();

void drawDebug(const float4x4 &viewMatrix, const float4x4 &projectionMatrix);
void debugDrawOctree(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, Octree tree);
void debugDrawQuad(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 halfVector);
void debugDrawLine(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 rayVector);

float rayOctreeIntersection(float3 rayOrigin, float3 rayVec, Octree oct);

float degreeToRad(float degree);
float radToDegree(float rad);
float3 sphericalToCartesian(float theta, float phi, float r);

void setFog(GLuint shaderProgram);

void initGL()
{
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	/* Initialize GLEW; this gives us access to OpenGL Extensions.
	 */
	glewInit();  

	/* Print information about OpenGL and ensure that we've got at a context 
	 * that supports least OpenGL 3.0. Then setup the OpenGL Debug message
	 * mechanism.
	 */
	startupGLDiagnostics();
	setupGLDebugMessages();

	/* Initialize DevIL, the image library that we use to load textures. Also
	 * tell IL that we intent to use it with OpenGL.
	 */
	ilInit();
	ilutRenderer(ILUT_OPENGL);

	/* Workaround for AMD. It might no longer be necessary, but I dunno if we
	 * are ever going to remove it. (Consider it a piece of living history.)
	 */
	if( !glBindFragDataLocation )
	{
		glBindFragDataLocation = glBindFragDataLocationEXT;
	}

	/* As a general rule, you shouldn't need to change anything before this 
	 * comment in initGL().
	 */


	sunCamera = new PerspectiveCamera(lightPosition, make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45.0f, 1.0f, 280.0f, 600.0f);
	playerCamera = new PerspectiveCamera(
		carLoc.location + sphericalToCartesian(camera_theta, camera_phi, camera_r),
		carLoc.location + make_vector(0.0f, camera_target_altitude, 0.0f),
		make_vector(0.0f, 1.0f, 0.0f),
		45.0f, float(w) / float(h), 0.1f, 1000.0f
		);


	skybox = new Skybox(playerCamera);
	skybox->init("scenes/posx.jpg", "scenes/negx.jpg", "scenes/posy.jpg", "scenes/posy.jpg", "scenes/negz.jpg", "scenes/posz.jpg");

	//*************************************************************************
	//	Load shaders
	//*************************************************************************
	shaderProgram = loadShaderProgram("shaders/simple.vert", "shaders/simple.frag");
	glBindAttribLocation(shaderProgram, 0, "position"); 	
	glBindAttribLocation(shaderProgram, 2, "texCoordIn");
	glBindAttribLocation(shaderProgram, 1, "normalIn");
	glBindFragDataLocation(shaderProgram, 0, "fragmentColor");
	linkShaderProgram(shaderProgram);


	//*************************************************************************
	// Load the models from disk
	//*************************************************************************
	logger.logInfo("Started loading models.");

	world.loadMesh("scenes/world.obj");
	world.m_modelMatrix = make_identity<float4x4>();

	factory.loadMesh("scenes/test.obj");
	factory.m_modelMatrix = make_translation(make_vector(-15.0f, 6.0f, 0.0f)) * make_rotation_y<float4x4>(M_PI / 180 * 90) * make_scale<float4x4>(make_vector(2.0f, 2.0f, 2.0f));
	
	spider.loadMesh("scenes/spider.obj");
	spider.m_modelMatrix = make_translation(make_vector(40.0f, 2.0f, 0.0f)) * make_rotation_x<float4x4>(M_PI / 180 * 0) *  make_scale<float4x4>(0.1f);
	
	water.loadMesh("../scenes/water.obj");
	water.m_modelMatrix = make_translation(make_vector(0.0f, -6.0f, 0.0f));

	car.loadMesh("scenes/car.obj");
	

	logger.logInfo("Finished loading models.");

	logger.logInfo("Started creating octree");
	high_resolution_clock::time_point start = high_resolution_clock::now();

	//float3 halfVector = (aabb_coll.maxV - aabb_coll.minV) / 2;
	//halfVector.x = fabs(halfVector.x);
	//halfVector.y = fabs(halfVector.y);
	//halfVector.z = fabs(halfVector.z);
	//float3 origin = aabb_coll.maxV - halfVector;
	float3 origin = make_vector(0.0f, 0.0f, 0.0f);
	float3 halfVector = make_vector(200.0f, 200.0f, 200.0f); //TODO

	octTree = new Octree(origin, halfVector, 0);

	collider = new Collider(octTree);
	collider->addMesh(&world);
	collider->addMesh(&water);
	collider->addMesh(&factory);
	collider->addMesh(&spider);
	collider->insertAll(); //TODO enlargen octrees afterhand instead

	high_resolution_clock::time_point end = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(end - start);
	
	logger.logInfo("Created octree in : " + std::to_string(time_span.count()) + " seconds.");

	//*************************************************************************
	// Generate shadow map frame buffer object
	//*************************************************************************
	logger.logInfo("Generating OpenGL data.");

	sbo.shaderProgram = loadShaderProgram("shaders/shadowMap.vert", "shaders/shadowMap.frag");
		glBindAttribLocation(sbo.shaderProgram, 0, "position");
		glBindFragDataLocation(sbo.shaderProgram, 0, "fragmentColor");
	linkShaderProgram(sbo.shaderProgram);


	sbo.width = SHADOW_MAP_RESOLUTION;
	sbo.height = SHADOW_MAP_RESOLUTION;
	
	glGenFramebuffers(1, &sbo.id);
	glBindFramebuffer(GL_FRAMEBUFFER, sbo.id);

	glGenTextures(1, &sbo.texture);
	glBindTexture(GL_TEXTURE_2D, sbo.texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float4 zeros = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &zeros.x);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sbo.texture, 0);

	//Disable reading color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//*************************************************************************
	// Create post process Fbo
	//*************************************************************************
	postFxShader = loadShaderProgram("shaders/postFx.vert", "shaders/postFx.frag");
	verticalBlurShader = loadShaderProgram("shaders/postFx.vert", "shaders/vertical_blur.frag");
	horizontalBlurShader = loadShaderProgram("shaders/postFx.vert", "shaders/horizontal_blur.frag");
	cutoffShader = loadShaderProgram("shaders/postFx.vert", "shaders/cutoff.frag");

	glBindAttribLocation(postFxShader, 0, "position");
	glBindFragDataLocation(postFxShader, 0, "fragmentColor");

	linkShaderProgram(postFxShader);
	linkShaderProgram(verticalBlurShader);
	linkShaderProgram(horizontalBlurShader);
	linkShaderProgram(cutoffShader);

	postProcessFbo = createPostProcessFbo(w, h);
	verticalBlurFbo = createPostProcessFbo(w, h);
	horizontalBlurFbo = createPostProcessFbo(w, h);
	cutOffFbo = createPostProcessFbo(w, h);

	//*************************************************************************
	// Load cube map texture
	//*************************************************************************
	reflectionCubeMap = new CubeMapTexture("scenes/posx.jpg", "scenes/negx.jpg", "scenes/posy.jpg", "scenes/posy.jpg", "scenes/negz.jpg", "scenes/posz.jpg");

	//X
	cubeMapCameras[0] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(100.0f, 3.0f, 0.0f),  make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);
	cubeMapCameras[1] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(-100.0f,3.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);

	//Y
	cubeMapCameras[2] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f , 100.1f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);
	cubeMapCameras[3] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, -100.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);

	//Z
	cubeMapCameras[4] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, 100.0f), make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);
	cubeMapCameras[5] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, -100.0f), make_vector(0.0f, -1.0f, 0.0f), 90.0f, 1, 0.1f, 1000.0f);

	cMapAll.width = w;
	cMapAll.height = h;

	glGenFramebuffers(1, &cMapAll.id);
	glBindFramebuffer(GL_FRAMEBUFFER, cMapAll.id);

	glGenTextures(1, &cMapAll.texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cMapAll.texture);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,	GL_RENDERBUFFER, depthBuffer);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cMapAll.texture, 0);

	cMapAll.shaderProgram = shaderProgram;
	
	//Cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	logger.logInfo("Generating OpenGL data completed.");
}




Fbo createPostProcessFbo(int width, int height) {

	Fbo fbo;

	fbo.width = width;
	fbo.height = height;

	glGenFramebuffers(1, &fbo.id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);

	glGenTextures(1, &fbo.texture);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo.texture);

	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, fbo.texture, 0);

	glGenRenderbuffers(1, &fbo.depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.depthbuffer);

	return fbo;
}

void drawModel(Mesh &model, const float4x4 &modelMatrix, GLuint shaderProgram)
{
	setUniformSlow(shaderProgram, "modelMatrix", modelMatrix); 
	model.render();
}

/**
* In this function, add all scene elements that should cast shadow, that way
* there is only one draw call to each of these, as this function is called twice.
*/
void drawShadowCasters(GLuint shaderProgram)
{
	drawModel(world, world.m_modelMatrix, shaderProgram);
	setUniformSlow(shaderProgram, "object_reflectiveness", 1.5f); 

	float3 frontDir = normalize(carLoc.frontDir);
	float3 rightDir = normalize(cross(frontDir, vUp));

	float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), frontDir)));
	float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), rightDir)));
	
	Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
	Quaternion qatY = make_quaternion_axis_angle(vUp, carLoc.angley);
	Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-carLoc.angley) * frontDir, anglez);

	drawModel(car,
		make_translation(carLoc.location)
		* makematrix(qatX)  
		* makematrix(qatY) 
		* makematrix(qatZ),
		shaderProgram);
	setUniformSlow(shaderProgram, "object_reflectiveness", 0.0f); 
	drawModel(factory, factory.m_modelMatrix, shaderProgram);

	drawModel(spider, spider.m_modelMatrix , shaderProgram);
}

void drawShadowMap(Fbo sbo, float4x4 viewProjectionMatrix) {
	glBindFramebuffer(GL_FRAMEBUFFER, sbo.id);
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5, 2.0);

	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	glUseProgram(sbo.shaderProgram);

	setUniformSlow(sbo.shaderProgram, "viewProjectionMatrix", viewProjectionMatrix);

	drawShadowCasters(sbo.shaderProgram);

	//CLEANUP
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(currentProgram);
}

void drawCubeMap(Fbo fbo) {
	int w = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	int h = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);
	glViewport(0, 0, w, h);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	glUseProgram(fbo.shaderProgram);

	float4x4 projectionMatrix = perspectiveMatrix(90.0f, 1, 2.0f, 1000.0f);

	for (int i = 0; i < 6; i++) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, fbo.texture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float4x4 viewMatrix = cubeMapCameras[i]->getViewMatrix();

		setUniformSlow(shaderProgram, "projectionMatrix", projectionMatrix);
		setUniformSlow(shaderProgram, "viewMatrix", viewMatrix);
		setUniformSlow(shaderProgram, "inverseViewNormalMatrix", transpose(viewMatrix));

		drawModel(water, water.m_modelMatrix, shaderProgram); 
		drawModel(factory, factory.m_modelMatrix, shaderProgram);

		drawModel(world, world.m_modelMatrix, shaderProgram);
	}

	// CLEAN UP
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(currentProgram);

}

void drawScene(void)
{
	
	// enable back face culling.
	glEnable(GL_CULL_FACE);

	//*************************************************************************
	// Render shadow map
	//*************************************************************************

	int w = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	int h = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

	float4x4 lightViewMatrix = sunCamera->getViewMatrix();
	float4x4 lightProjectionMatrix = sunCamera->getProjectionMatrix();
	float4x4 lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;

	drawShadowMap(sbo, lightViewProjectionMatrix);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFbo.id);

	//*************************************************************************
	// Render the scene from the cameras viewpoint, to the default framebuffer
	//*************************************************************************
	glClearColor(0.2,0.2,0.8,1.0);						
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glViewport(0, 0, w, h);								
	// Use shader and set up uniforms
	glUseProgram( shaderProgram );			

	//Calculate camera matrix
	playerCamera->setLookAt(carLoc.location + make_vector(0.0f, camera_target_altitude, 0.0f));
	playerCamera->setPosition(carLoc.location + sphericalToCartesian(camera_theta, camera_phi, camera_r));


	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	if (camera == 6) {
		viewMatrix			= playerCamera->getViewMatrix();
		projectionMatrix	= playerCamera->getProjectionMatrix();
	}
	else if (camera == 7) {
		viewMatrix			= lightViewMatrix;
		projectionMatrix	= lightProjectionMatrix;
	}
	else
	{
		viewMatrix			= cubeMapCameras[camera]->getViewMatrix();
		projectionMatrix	= cubeMapCameras[camera]->getProjectionMatrix();
	}

	float4x4 lightMatrix = 
		make_translation({ 0.5, 0.5, 0.5 }) * 
		make_scale<float4x4>(make_vector(0.5f, 0.5f, 0.5f)) *
		lightViewProjectionMatrix * inverse(viewMatrix);

	//Sets matrices
	setUniformSlow(shaderProgram, "viewMatrix", viewMatrix);
	setUniformSlow(shaderProgram, "projectionMatrix", projectionMatrix);
	setUniformSlow(shaderProgram, "lightpos", lightPosition);
	setUniformSlow(shaderProgram, "lightMatrix", lightMatrix);
	setUniformSlow(shaderProgram, "inverseViewNormalMatrix", transpose(viewMatrix));

	//Sets fog
	setFog(shaderProgram);

	//Set shadowmap
	setUniformSlow(shaderProgram, "shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sbo.texture);

	//Set cube map
	setUniformSlow(shaderProgram, "cubeMap", 2);
	
	//reflectionCubeMap->bind(GL_TEXTURE2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cMapAll.texture);
	
	//Render models
	drawModel(water, water.m_modelMatrix, shaderProgram);
	drawShadowCasters(shaderProgram);

	//Render debuggers
	drawDebug(viewMatrix, projectionMatrix);

	//Render skybox
	skybox->render();

	//Render cube map
	drawCubeMap(cMapAll);

	//Perform post processing
	renderPostProcess();
	
	//Cleanup
	glUseProgram( 0 );	
}

void renderPostProcess() {
	int w = glutGet((GLenum)GLUT_WINDOW_WIDTH);
	int h = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

	blurImage();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glClearColor(0.6, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(postFxShader);
	setUniformSlow(postFxShader, "frameBufferTexture", 0);
	setUniformSlow(postFxShader, "blurredFrameBufferTexture", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, postProcessFbo.texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, verticalBlurFbo.texture);

	setUniformSlow(postFxShader, "time", currentTime);

	drawFullScreenQuad();
}

void blurImage() {
	//CUTOFF
	glUseProgram(cutoffShader);
	glBindFramebuffer(GL_FRAMEBUFFER, cutOffFbo.id);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setUniformSlow(cutoffShader, "time", currentTime);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, postProcessFbo.texture);
	
	drawFullScreenQuad();

	//HORIZONTAL
	glBindFramebuffer(GL_FRAMEBUFFER, horizontalBlurFbo.id);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(horizontalBlurShader);

	setUniformSlow(horizontalBlurShader, "frameBufferTexture", 0);
	setUniformSlow(horizontalBlurShader, "time", currentTime);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cutOffFbo.texture);
	drawFullScreenQuad();

	//VERTICAL
	glBindFramebuffer(GL_FRAMEBUFFER, verticalBlurFbo.id);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(verticalBlurShader);

	setUniformSlow(verticalBlurShader, "frameBufferTexture", 0);
	setUniformSlow(verticalBlurShader, "time", currentTime);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, horizontalBlurFbo.texture);
	drawFullScreenQuad();
}

void checkIntersection() {
	float3 upVec = make_vector(0.0f, 1.0f, 0.0f);

	//Calculate intersections
	float3x3 rot = make_rotation_y<float3x3>(carLoc.angley);
	float a = collider->rayIntersection(carLoc.location + rot * carLoc.wheel1, -upVec);
	float b = collider->rayIntersection(carLoc.location + rot * carLoc.wheel2, -upVec);
	float c = collider->rayIntersection(carLoc.location + rot * carLoc.wheel3, -upVec);
	float d = collider->rayIntersection(carLoc.location + rot * carLoc.wheel4, -upVec);

	if (a == 0 && b == 0 && c == 0 && d == 0) {
		return;
	}
	//Calculate 3d points of intersection
	float3 af = carLoc.wheel1 - (upVec * a);
	float3 bf = carLoc.wheel2 - (upVec * b);
	float3 cf = carLoc.wheel3 - (upVec * c);
	float3 df = carLoc.wheel4 - (upVec * d);

	//Calculate new up vector
	float3 vABa = af - bf;
	float3 vCBa = cf - bf;
	float3 newUpa = cross(vABa, vCBa);

	float3 vABb = bf - cf;
	float3 vCBb = df - cf;
	float3 newUpb = cross(vABb, vCBb);

	float3 halfVector = normalize(newUpa - newUpb);
	carLoc.upDir = -(rot * halfVector);

	

	//Change wheel locations 
	carLoc.wheel1 += upVec * a;
	carLoc.wheel2 += upVec * b;
	carLoc.wheel3 += upVec * c;
	carLoc.wheel4 += upVec * d;

	float3 frontDir = normalize(carLoc.frontDir);
	float3 rightDir = normalize(cross(frontDir, vUp));

	float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), frontDir)));
	float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), rightDir)));
	
	Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
	Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-carLoc.angley) * frontDir, anglez);

	float4 newLoc = makematrix(qatX) * makematrix(qatZ) * make_vector4(carLoc.wheel1, 1.0f);

	carLoc.location += make_vector(0.0f, carLoc.wheel1.y + (carLoc.wheel1.y - newLoc.y), 0.0f);
	printf("%f\n", carLoc.wheel1.y + (carLoc.wheel1.y - newLoc.y));
}


void display(void)
{
	drawScene();
	
	glutSwapBuffers();  // swap front and back buffer. This frame will now be displayed.
	CHECK_GL_ERROR();
}

void handleKeys(unsigned char key, int /*x*/, int /*y*/)
{
	switch(key)
	{
	case 27:    /* ESC */
		exit(0); /* dirty exit */
		break;   /* unnecessary, I know */
	case 32:    /* space */
		paused = !paused;
		break;
	case 'w':
	case 'W':
		keysDown[(int)'w'] = true;
		break;
	case 'a':
	case 'A': 
		keysDown[(int)'a'] = true;
		break;
	case 's':   
	case 'S':
		keysDown[(int)'s'] = true;
		break;
	case 'd':   
	case 'D':
		keysDown[(int)'d'] = true;
		break;
	}
}

void handleKeysRelease(unsigned char key, int /*x*/, int /*y*/)
{
	switch (key)
	{
	case 27:    /* ESC */
		exit(0); /* dirty exit */
		break;   /* unnecessary, I know */
	case 32:    /* space */
		paused = !paused;
		break;
	case 'w':
	case 'W':
		keysDown[(int)'w'] = false;
		break;
	case 'a':
	case 'A':
		keysDown[(int)'a'] = false;
		break;
	case 's':
	case 'S':
		keysDown[(int)'s'] = false;
		break;
	case 'd':
	case 'D':
		keysDown[(int)'d'] = false;
		break;
	}
}

void handleSpecialKeys(int key, int /*x*/, int /*y*/)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		printf("Left arrow\n");
		camera = (camera + 1) % 8;
		break;
	case GLUT_KEY_RIGHT:
		camera = (camera - 1);
		camera = camera == -1 ? 7 : camera;
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	// reset the previous position, such that we only get movement performed after the button
	// was pressed.
	prev_x = x;
	prev_y = y;

	bool buttonDown = state == GLUT_DOWN;

	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		leftDown = buttonDown;
		break;
	case GLUT_MIDDLE_BUTTON:
		middleDown = buttonDown;
		break;
	case GLUT_RIGHT_BUTTON: 
		rightDown = buttonDown;
	default:
		break;
	}
}

void motion(int x, int y)
{
	int delta_x = x - prev_x;
	int delta_y = y - prev_y;

	if(middleDown)
	{
		camera_r -= float(delta_y) * 0.3f;
		// make sure cameraDistance does not become too small
		camera_r = max(0.1f, camera_r);
	}
	if(leftDown)
	{
		camera_phi	-= float(delta_y) * 0.3f * float(M_PI) / 180.0f;
		camera_phi = min(max(0.01f, camera_phi), float(M_PI) - 0.01f);
		camera_theta -= float(delta_x) * 0.3f * float(M_PI) / 180.0f;
	}

	if(rightDown)
	{
		camera_target_altitude += float(delta_y) * 0.1f; 
	}
	prev_x = x;
	prev_y = y;
}

void tick() {
	if (keysDown[(int)'w']) {
		float3 term = carLoc.frontDir * carLoc.moveSpeed;
		carLoc.location += term;
		hasChanged = true;

	}if (keysDown[(int)'s']) {
		float3 term = carLoc.frontDir * carLoc.moveSpeed;
		carLoc.location -= term;
		hasChanged = true;

	}if (keysDown[(int)'a'] && (keysDown[(int)'w'] || keysDown[(int)'s'])) {
		carLoc.angley += carLoc.rotationSpeed;
		carLoc.frontDir = make_rotation_y<float3x3>(carLoc.rotationSpeed) * carLoc.frontDir;
		camera_theta += carLoc.rotationSpeed;
		hasChanged = true;

	}if (keysDown[(int)'d'] && (keysDown[(int)'w'] || keysDown[(int)'s'])) {
		carLoc.angley -= carLoc.rotationSpeed;
		carLoc.frontDir = make_rotation_y<float3x3>(-carLoc.rotationSpeed) * carLoc.frontDir;
		camera_theta -= carLoc.rotationSpeed;
		hasChanged = true;
	}
}

void setFog(GLuint shaderProgram) {
	setUniformSlow(shaderProgram, "fog.iEquation",	FogParams::fEquation);
	setUniformSlow(shaderProgram, "fog.fDensity",	FogParams::fDensity);
	setUniformSlow(shaderProgram, "fog.fEnd",		FogParams::fEnd);
	setUniformSlow(shaderProgram, "fog.fStart",		FogParams::fStart);
	setUniformSlow(shaderProgram, "fog.vColor",		FogParams::vColor);
}

void idle( int v )
{
	float time = (1000 / TICK_PER_SECOND) - (float(glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw));
	if ( time < 0) {
		glutTimerFunc(1000 / TICK_PER_SECOND, idle, 0);
		timeSinceDraw = float(glutGet(GLUT_ELAPSED_TIME));
		static float startTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
		// Here is a good place to put application logic.
		currentTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f - startTime;


		// rotate light around X axis, sunlike fashion.
		// do one full revolution every 20 seconds.
		float4x4 rotateLight = make_rotation_x<float4x4>(2.0f * M_PI * currentTime / 20.0f);
		// rotate and update global light position.
		lightPosition = make_vector3(rotateLight * make_vector(30.1f, 450.0f, 0.1f, 1.0f));
		sunCamera->setPosition(lightPosition);

		tick();

		if (hasChanged){
			checkIntersection();
			hasChanged = false;
		}

		glutPostRedisplay();
	}
	else {
		glutTimerFunc(time, idle, 0);
	}
}


int main(int argc, char *argv[])
{
#	if defined(__linux__)
	linux_initialize_cwd();
#	endif // ! __linux__


	glutInit(&argc, argv);

	/* Request a double buffered window, with a sRGB color buffer, and a depth
	 * buffer. Also, request the initial window size to be 800 x 600.
	 *
	 * Note: not all versions of GLUT define GLUT_SRGB; fall back to "normal"
	 * RGB for those versions.
	 */
#	if defined(GLUT_SRGB)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_SRGB | GLUT_DEPTH);
#	else // !GLUT_SRGB
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	printf( "--\n" );
	printf( "-- WARNING: your GLUT doesn't support sRGB / GLUT_SRGB\n" );
#	endif // ~ GLUT_SRGB
	glutInitWindowSize(SCREEN_WIDTH,SCREEN_HEIGHT);

	/* Require at least OpenGL 3.0. Also request a Debug Context, which allows
	 * us to use the Debug Message API for a somewhat more humane debugging
	 * experience.
	 */
	glutInitContextVersion(3,0);
	glutInitContextFlags(GLUT_DEBUG);

	/* Request window
	 */
	glutCreateWindow("Project");

	/* Set callbacks that respond to various events. Most of these should be
	 * rather self-explanatory (i.e., the MouseFunc is called in response to
	 * a mouse button press/release). The most important callbacks are however
	 *
	 *   - glutDisplayFunc : called whenever the window is to be redrawn
	 *   - glutIdleFunc : called repeatedly
	 *
	 * The window is redrawn once at startup (at the beginning of
	 * glutMainLoop()), and whenever the window changes (overlap, resize, ...).
	 * To repeatedly redraw the window, we need to manually request that via
	 * glutPostRedisplay(). We call this from the glutIdleFunc.
	 */
	glutTimerFunc(50,idle, 0);
	glutDisplayFunc(display);

	glutKeyboardFunc(handleKeys); // standard key is pressed/released
	glutKeyboardUpFunc(handleKeysRelease);
	glutSpecialFunc(handleSpecialKeys); // "special" key is pressed/released
	glutMouseFunc(mouse); // mouse button pressed/released
	glutMotionFunc(motion); // mouse moved *while* any button is pressed

	/* Now that we should have a valid GL context, perform our OpenGL 
	 * initialization, before we enter glutMainLoop().
	 */
	initGL();

	/* If sRGB is available, enable rendering in sRGB. Note: we should do
	 * this *after* initGL(), since initGL() initializes GLEW.
	 */
	glEnable(GL_FRAMEBUFFER_SRGB);

	/* Start the main loop. Note: depending on your GLUT version, glutMainLoop()
	 * may never return, but only exit via std::exit(0) or a similar method.
	 */
	glutMainLoop();


	return 0;          
}

void drawFullScreenQuad()
{
	static GLuint vertexArrayObject = 0;
	static int nofVertices = 4;

	// do this initialization first time the function is called... somewhat dodgy, but works for demonstration purposes
	if (vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &vertexArrayObject);
		static const float2 positions[] = {
				{ -1.0f, -1.0f },
				{ 1.0f, -1.0f },
				{ 1.0f, 1.0f },
				{ -1.0f, 1.0f },
		};
		createAddAttribBuffer(vertexArrayObject, positions, sizeof(positions), 0, 2, GL_FLOAT);
	}

	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_QUADS, 0, nofVertices);
}

void drawDebug(const float4x4 &viewMatrix, const float4x4 &projectionMatrix) {
	/*debugDrawOctree(viewMatrix, projectionMatrix, t);
	debugDrawLight(viewMatrix, projectionMatrix, lightPosition);
	debugDrawQuad(viewMatrix, projectionMatrix, carLoc.location + make_vector(0.2f, 1.2f, 0.0f), make_vector(1.0f, 1.0f, 1.5f));
	float3x3 rot = make_rotation_y<float3x3>(carLoc.angley);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location + rot * carLoc.wheel1, -carLoc.upDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location + rot * carLoc.wheel2, -carLoc.upDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location + rot * carLoc.wheel3, -carLoc.upDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location + rot * carLoc.wheel4, -carLoc.upDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location, carLoc.upDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location, carLoc.frontDir);
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location, normalize(cross(carLoc.frontDir, make_vector(0.0f, 1.0f, 0.0f))));
	debugDrawLine(viewMatrix, projectionMatrix, carLoc.location, vUp);*/
}

void debugDrawLine(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 rayVector) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLint temp;
	glColor3f(1.0, 1.0, 0.0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projectionMatrix.c1.x);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&viewMatrix.c1.x);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 p[8];
	p[0] = origin;
	p[1] = origin + (normalize(rayVector) * 5);

	glBegin(GL_LINES);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glEnd();
	glUseProgram(temp);
	glPopAttrib();
}

void debugDrawQuad(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, float3 origin, float3 halfVector) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLint temp;
	glColor3f(1.0, 1.0, 0.0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projectionMatrix.c1.x);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&viewMatrix.c1.x);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 p[8];
	p[0] = origin + (halfVector * make_vector(1.0f, 1.0f, 1.0f));
	p[1] = origin + (halfVector * make_vector(-1.0f, 1.0f, 1.0f));

	p[2] = origin + (halfVector * make_vector(1.0f, -1.0f, 1.0f));
	p[3] = origin + (halfVector * make_vector(-1.0f, -1.0f, 1.0f));

	p[4] = origin + (halfVector * make_vector(1.0f, 1.0f, -1.0f));
	p[5] = origin + (halfVector * make_vector(-1.0f, 1.0f, -1.0f));

	p[6] = origin + (halfVector * make_vector(1.0f, -1.0f, -1.0f));
	p[7] = origin + (halfVector * make_vector(-1.0f, -1.0f, -1.0f));


	glBegin(GL_LINES);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	///////
	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[6].x, p[6].y, p[6].z);

	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[5].x, p[5].y, p[5].z);

	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[3].x, p[3].y, p[3].z);

	///////
	glVertex3f(p[5].x, p[5].y, p[5].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[5].x, p[5].y, p[5].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	///////
	glVertex3f(p[3].x, p[3].y, p[3].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[3].x, p[3].y, p[3].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	///////
	glVertex3f(p[6].x, p[6].y, p[6].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	glVertex3f(p[6].x, p[6].y, p[6].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	glEnd();
	glUseProgram(temp);
	glPopAttrib();
}

void debugDrawOctree(const float4x4 &viewMatrix, const float4x4 &projectionMatrix, Octree tree)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLint temp;
	glColor3f(1.0, 1.0, 0.0);
	glGetIntegerv(GL_CURRENT_PROGRAM, &temp);
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projectionMatrix.c1.x);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&viewMatrix.c1.x);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 p[8];
	p[0] = tree.origin + (tree.halfVector * make_vector(1.0f, 1.0f, 1.0f));
	p[1] = tree.origin + (tree.halfVector * make_vector(-1.0f, 1.0f, 1.0f));

	p[2] = tree.origin + (tree.halfVector * make_vector(1.0f, -1.0f, 1.0f));
	p[3] = tree.origin + (tree.halfVector * make_vector(-1.0f, -1.0f, 1.0f));

	p[4] = tree.origin + (tree.halfVector * make_vector(1.0f, 1.0f, -1.0f));
	p[5] = tree.origin + (tree.halfVector * make_vector(-1.0f, 1.0f, -1.0f));

	p[6] = tree.origin + (tree.halfVector * make_vector(1.0f, -1.0f, -1.0f));
	p[7] = tree.origin + (tree.halfVector * make_vector(-1.0f, -1.0f, -1.0f));


	glBegin(GL_LINES);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	///////
	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[6].x, p[6].y, p[6].z);

	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[5].x, p[5].y, p[5].z);

	glVertex3f(p[7].x, p[7].y, p[7].z);
	glVertex3f(p[3].x, p[3].y, p[3].z);

	///////
	glVertex3f(p[5].x, p[5].y, p[5].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[5].x, p[5].y, p[5].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	///////
	glVertex3f(p[3].x, p[3].y, p[3].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);

	glVertex3f(p[3].x, p[3].y, p[3].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	///////
	glVertex3f(p[6].x, p[6].y, p[6].z);
	glVertex3f(p[4].x, p[4].y, p[4].z);

	glVertex3f(p[6].x, p[6].y, p[6].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);

	glEnd();
	glUseProgram(temp);
	glPopAttrib();
	

	std::vector<Octree> children;
	tree.getChildren(&children);

	if (children.size() != 0) {
		for (int i = 0; i < 8; i++) {
			debugDrawOctree(viewMatrix, projectionMatrix, children[i]);
		}
	}
}

float degreeToRad(float degree) {
	return degree * M_PI / 180;
}

float radToDegree(float rad) {
	return rad * 180 / M_PI;
}

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r)
{
	return make_vector(r * sinf(theta)*sinf(phi),
		r * cosf(phi),
		r * cosf(theta)*sinf(phi));
}

/* TIME COUNT
high_resolution_clock::time_point start = high_resolution_clock::now();
high_resolution_clock::time_point end = high_resolution_clock::now();
duration<double> time_span = duration_cast<duration<double>>(end - start);
printf("TIME FOR COLL:%f\n", time_span.count());
*/