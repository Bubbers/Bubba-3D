#include "Renderer.h"
#include <sstream>
#include <SFML/Window.hpp>
#include <Globals.h>
#include <JoystickTranslator.h>
#include "ResourceManager.h"
#include "constants.h"
#include "GameObject.h"
#include "CubeMapTexture.h"
#include "Logger.h"
#include "Camera.h"
#include "Scene.h"




namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


Renderer::Renderer(int width, int height)
{
#	if defined(__linux__)
  	linux_initialize_cwd();
#	endif // ! __linux__

	sf::ContextSettings settings = sf::ContextSettings(32, 8, 0, 3, 3);

	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Debug | sf::ContextSettings::Core;
	window = new sf::Window(sf::VideoMode(width,height),"Super-Bubba-Awesome-Space",sf::Style::Default,settings);
	glEnable(GL_FRAMEBUFFER_SRGB);
	window->setFramerateLimit(60);
	JoystickTranslator::getInstance()->cacheControlsMappings();
    initGL();
	resize(width, height);
}


Renderer::~Renderer()
{
}

void Renderer::start() {
	bool running = true;
	while (running)
	{
		// handle events
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::MouseMoved) {
				Globals::set(Globals::Key::MOUSE_WINDOW_X, event.mouseMove.x);
				Globals::set(Globals::Key::MOUSE_WINDOW_Y, event.mouseMove.y);
			}
			else if (event.type == sf::Event::JoystickDisconnected || event.type == sf::Event::JoystickConnected) {
				JoystickTranslator::getInstance()->cacheControlsMappings();
			}
			else if (event.type == sf::Event::Resized)
			{
				resize(event.size.width, event.size.height);
			}

		}

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		idleMethod(0);
		displayMethod();

		// end the current frame (internally swaps the front and back buffers)
		window->display();
	}
}

void Renderer::resize(unsigned int width, unsigned int height) {
	Globals::set(Globals::Key::WINDOW_HEIGHT, height);
	Globals::set(Globals::Key::WINDOW_WIDTH, width);

	// adjust the viewport when the window is resized
	glViewport(0, 0, width, height);

	postProcessFbo = createPostProcessFbo(width, height);
	verticalBlurFbo = createPostProcessFbo(width, height);
	horizontalBlurFbo = createPostProcessFbo(width, height);
	cutOffFbo = createPostProcessFbo(width, height);
}

void Renderer::render() {
}

void Renderer::setDisplayMethod(void(*display)(void)) {
	displayMethod = display;
}

void Renderer::setIdleMethod(void(*idle)(int), int maxFps) {
	idleMethod = idle;
	this->maxFps = maxFps;
}

sf::Window* Renderer::getWindow() {
	return window;
}

void Renderer::drawModel(IDrawable &model, Shader* shaderProgram)
{
	shaderProgram->use();
	model.render();
}

void Renderer::drawScene(Camera *camera, Scene *scene, float currentTime)
{
	Renderer::currentTime = currentTime;

	float4x4 viewMatrix = camera->getViewMatrix();
	float4x4 projectionMatrix = camera->getProjectionMatrix();
	float4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;



	// enable back face culling.
	glEnable(GL_CULL_FACE);

	//*************************************************************************
	// Render shadow map
	//*************************************************************************
	float4x4 lightMatrix = make_identity<float4x4>();

	if (scene->shadowMapCamera != NULL) {
		float4x4 lightViewMatrix = scene->shadowMapCamera->getViewMatrix();
		float4x4 lightProjectionMatrix = scene->shadowMapCamera->getProjectionMatrix();
		float4x4 lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;

		lightMatrix = make_translation(make_vector( 0.5f, 0.5f, 0.5f )) * make_scale<float4x4>(make_vector(0.5f, 0.5f, 0.5f)) * lightViewProjectionMatrix * inverse(viewMatrix);

		drawShadowMap(sbo, lightViewProjectionMatrix, scene);
	}

	//*************************************************************************
	// Render the scene from the cameras viewpoint, to the default framebuffer
	//*************************************************************************
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFbo.id);
	glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int w = Globals::get(Globals::Key::WINDOW_WIDTH);
	int h = Globals::get(Globals::Key::WINDOW_HEIGHT);
	glViewport(0, 0, w, h);
	// Use shader and set up uniforms
	shaderProgram->use();

	shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 0 * sizeof(float4x4), sizeof(float4x4), &(viewMatrix.c1.x));
	shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 1 * sizeof(float4x4), sizeof(float4x4), &(projectionMatrix.c1.x));
	shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 2 * sizeof(float4x4), sizeof(float4x4), &(viewProjectionMatrix.c1.x));

	//Sets matrices
	shaderProgram->setUniformMatrix4fv("lightMatrix", lightMatrix);
	shaderProgram->setUniformMatrix4fv("inverseViewNormalMatrix", transpose(viewMatrix));
	shaderProgram->setUniform3f("viewPosition", camera->getPosition());
	shaderProgram->setUniformMatrix4fv("viewMatrix", viewMatrix);

	setLights(shaderProgram, scene);

	setFog(shaderProgram);

	//Set shadowmap
	if (scene->shadowMapCamera != NULL) {
		shaderProgram->setUniform1i("shadowMap", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, sbo.texture);
	}

	//Set cube map
	if (scene->cubeMap != NULL) {
		shaderProgram->setUniform1i("cubeMap", 2);
		scene->cubeMap->bind(GL_TEXTURE2);
	}

	drawShadowCasters(shaderProgram, scene);
	drawTransparent(shaderProgram, scene);

	renderPostProcess();

	//Cleanup
	glUseProgram(0);

}

void Renderer::setLights(Shader* shaderProgram, Scene *scene) {
	//set dirlights
	shaderProgram->setUniform3f("directionalLight.colors.ambientColor", scene->directionalLight.ambientColor);
	shaderProgram->setUniform3f("directionalLight.colors.diffuseColor", scene->directionalLight.diffuseColor);
	shaderProgram->setUniform3f("directionalLight.colors.specularColor", scene->directionalLight.specularColor);
	shaderProgram->setUniform3f("directionalLight.direction", scene->directionalLight.direction);

	//set pointLights

	shaderProgram->setUniform1i("nrPointLights", (int)scene->pointLights.size());
	for (int i = 0; i < (int)scene->pointLights.size(); i++) {
		string name = std::string("pointLights[") + patch::to_string(i).c_str() + "]";
		shaderProgram->setUniform3f((name + ".position").c_str(), scene->pointLights[i].position);
		shaderProgram->setUniform3f((name + ".colors.ambientColor").c_str(), scene->pointLights[i].ambientColor);
		shaderProgram->setUniform3f((name + ".colors.diffuseColor").c_str(), scene->pointLights[i].diffuseColor);
		shaderProgram->setUniform3f((name + ".colors.specularColor").c_str(), scene->pointLights[i].specularColor);
		shaderProgram->setUniform1f((name + ".attenuation.constant").c_str(), scene->pointLights[i].attenuation.constant);
		shaderProgram->setUniform1f((name + ".attenuation.linear").c_str(), scene->pointLights[i].attenuation.linear);
		shaderProgram->setUniform1f((name + ".attenuation.exp").c_str(), scene->pointLights[i].attenuation.exp);
	}

	//set spotLights
	shaderProgram->setUniform1i("nrSpotLights", (int)scene->spotLights.size());
	for (int i = 0; i < (int)scene->spotLights.size(); i++) {
		string name = std::string("spotLights[") + patch::to_string(i).c_str() + "]";
		shaderProgram->setUniform3f((name + ".position").c_str(), scene->spotLights[i].position);
		shaderProgram->setUniform3f((name + ".colors.ambientColor").c_str(), scene->spotLights[i].ambientColor);
		shaderProgram->setUniform3f((name + ".colors.diffuseColor").c_str(), scene->spotLights[i].diffuseColor);
		shaderProgram->setUniform3f((name + ".colors.specularColor").c_str(), scene->spotLights[i].specularColor);
		shaderProgram->setUniform1f((name + ".attenuation.constant").c_str(), scene->spotLights[i].attenuation.constant);
		shaderProgram->setUniform1f((name + ".attenuation.linear").c_str(), scene->spotLights[i].attenuation.linear);
		shaderProgram->setUniform1f((name + ".attenuation.exp").c_str(), scene->spotLights[i].attenuation.exp);
		shaderProgram->setUniform3f((name + ".direction").c_str(), scene->spotLights[i].direction);
		shaderProgram->setUniform1f((name + ".cutoff").c_str(), scene->spotLights[i].cutOff);
		shaderProgram->setUniform1f((name + ".cutoffOuter").c_str(), scene->spotLights[i].outerCutOff);
	}
}

/**
* In this function, add all scene elements that should cast shadow, that way
* there is only one draw call to each of these, as this function is called twice.
*/
void Renderer::drawShadowCasters(Shader* shaderProgram, Scene *scene)
{
	for (unsigned int i = 0; i < scene->shadowCasters.size(); i++) {
		shaderProgram->setUniform1f("object_reflectiveness", (*scene->shadowCasters[i]).shininess);
		drawModel(*scene->shadowCasters[i], shaderProgram);
	}
}

void Renderer::drawTransparent(Shader* shaderProgram, Scene *scene)
{
	for (unsigned int i = 0; i < scene->transparentObjects.size(); i++) {
		shaderProgram->setUniform1f("object_reflectiveness", (*scene->transparentObjects[i]).shininess);
		drawModel(*scene->transparentObjects[i], shaderProgram);
	}
}

void Renderer::drawShadowMap(Fbo sbo, float4x4 viewProjectionMatrix, Scene *scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, sbo.id);
	glViewport(0, 0, SHADOW_MAP_RESOLUTION, SHADOW_MAP_RESOLUTION);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5, 2.0);

	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	sbo.shaderProgram->use();
	sbo.shaderProgram->setUniformMatrix4fv("viewProjectionMatrix", viewProjectionMatrix);

	for (unsigned int i = 0; i < scene->shadowCasters.size(); i++) {
		sbo.shaderProgram->setUniform1f("object_reflectiveness", (*scene->shadowCasters[i]).shininess);
        (*scene->shadowCasters[i]).renderShadow(sbo.shaderProgram);
   	}

	//CLEANUP
	glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(currentProgram);
}

void Renderer::setFog(Shader* shaderProgram) {
	if (effects.fog.fEquation == FOG_EQ::NONE){ return; }
	shaderProgram->setUniform1i("fog.iEquation",	effects.fog.fEquation);
	shaderProgram->setUniform1f("fog.fDensity",	effects.fog.fDensity);
	shaderProgram->setUniform1f("fog.fEnd",		effects.fog.fEnd);
	shaderProgram->setUniform1f("fog.fStart",    effects.fog.fStart);
	shaderProgram->setUniform3f("fog.vColor",	effects.fog.vColor);
}

void Renderer::initGL()
{
	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	/* Print information about OpenGL and ensure that we've got at a context
	* that supports least OpenGL 3.0. Then setup the OpenGL Debug message
	* mechanism.
	*/
	startupGLDiagnostics();
	setupGLDebugMessages();

	FreeImage_Initialise(true);

	/* Workaround for AMD. It might no longer be necessary, but I dunno if we
	* are ever going to remove it. (Consider it a piece of living history.)
	*/
	if (!glBindFragDataLocation)
	{
		glBindFragDataLocation = glBindFragDataLocationEXT;
	}

	//*************************************************************************
	//	Load shaders
	//*************************************************************************
	ResourceManager::loadShader("../shaders/simple.vert", "../shaders/simple.frag", SIMPLE_SHADER_NAME);

	shaderProgram = ResourceManager::getShader(SIMPLE_SHADER_NAME);
	shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);
	shaderProgram->initUniformBufferObject(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 3 * sizeof(float4x4), UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);


	//*************************************************************************
	// Generate shadow map frame buffer object
	//*************************************************************************
	Logger::logInfo("Generating OpenGL data.");

	ResourceManager::loadShader("../shaders/shadowMap.vert", "../shaders/shadowMap.frag", "SHADOW_SHADER");
	sbo.shaderProgram = ResourceManager::getShader("SHADOW_SHADER");

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

	string post_fx = "POST_FX_SHADER";
	string vert_blur = "VERTICAL_BLUR_SHADER";
	string hor_blur = "HORIZONTAL_BLUR_SHADER";
	string cutoff = "CUTOFF_SHADER";

	ResourceManager::loadShader("../shaders/postFx.vert", "../shaders/postFx.frag", post_fx);
	ResourceManager::loadShader("../shaders/postFx.vert", "../shaders/vertical_blur.frag", vert_blur);
	ResourceManager::loadShader("../shaders/postFx.vert", "../shaders/horizontal_blur.frag", hor_blur);
	ResourceManager::loadShader("../shaders/postFx.vert", "../shaders/cutoff.frag", cutoff);

	postFxShader = ResourceManager::getShader(post_fx);
	verticalBlurShader = ResourceManager::getShader(vert_blur);
	horizontalBlurShader = ResourceManager::getShader(hor_blur);
	cutoffShader = ResourceManager::getShader(cutoff);


	//Cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	Logger::logInfo("Generating OpenGL data completed.");
}

Fbo Renderer::createPostProcessFbo(int width, int height) {

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

void Renderer::renderPostProcess() {

	int w = Globals::get(Globals::WINDOW_WIDTH);
	int h = Globals::get(Globals::WINDOW_HEIGHT);

	blurImage();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glClearColor(0.6f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	postFxShader->use();
	postFxShader->setUniform1i("frameBufferTexture", 0);
	postFxShader->setUniform1i("blurredFrameBufferTexture", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, postProcessFbo.texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, verticalBlurFbo.texture);

	postFxShader->setUniform1f("time", currentTime);

	drawFullScreenQuad();
}

void Renderer::blurImage() { 
	if (!effects.blur.active) { return; }
	//CUTOFF

	int width = Globals::get(Globals::WINDOW_WIDTH);
	int height = Globals::get(Globals::WINDOW_HEIGHT);
	cutoffShader->use();
	glBindFramebuffer(GL_FRAMEBUFFER, cutOffFbo.id);
	glViewport(0, 0, width, height);
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cutoffShader->setUniform1f("cutAt", effects.blur.cutOff);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, postProcessFbo.texture);

	drawFullScreenQuad();

	//HORIZONTAL
	glBindFramebuffer(GL_FRAMEBUFFER, horizontalBlurFbo.id);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	horizontalBlurShader->use();

	horizontalBlurShader->setUniform1i("frameBufferTexture", 0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cutOffFbo.texture);
	drawFullScreenQuad();

	//VERTICAL
	glBindFramebuffer(GL_FRAMEBUFFER, verticalBlurFbo.id);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	verticalBlurShader->use();

	verticalBlurShader->setUniform1i("frameBufferTexture", 0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, horizontalBlurFbo.texture);
	drawFullScreenQuad();
}

void Renderer::drawFullScreenQuad()
{
	static GLuint vertexArrayObject = 0;

	// do this initialization first time the function is called... somewhat dodgy, but works for demonstration purposes
	if (vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &vertexArrayObject);
		static const float2 positions[] = {
				-1.0f, -1.0f,
				 1.0f,  1.0f,
				-1.0f,  1.0f,

				-1.0f, -1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f
		};
		createAddAttribBuffer(vertexArrayObject, positions, sizeof(positions), 0, 2, GL_FLOAT);
		GLuint pos_vbo;
		glGenBuffers(1, &pos_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

		glBindVertexArray(vertexArrayObject);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0 );
		glEnableVertexAttribArray(0);
		CHECK_GL_ERROR();
	}

	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

