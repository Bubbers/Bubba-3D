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
#include "Renderer.h"
#include <sstream>
#include <Globals.h>
#include "ResourceManager.h"
#include "constants.h"
#include "GameObject.h"
#include "CubeMapTexture.h"
#include "Logger.h"
#include "Camera.h"
#include "Scene.h"
#include "ShaderProgram.h"


namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::initRenderer(int width, int height) {
    initGL();
    resize(width, height);

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::resize(unsigned int width, unsigned int height) {
    // adjust the viewport when the window is resized
    glViewport(0, 0, width, height);

    postProcessFbo = createPostProcessFbo(width, height);
    verticalBlurFbo = createPostProcessFbo(width, height);
    horizontalBlurFbo = createPostProcessFbo(width, height);
}

void Renderer::drawModel(const std::shared_ptr<IDrawable> &model, std::shared_ptr<ShaderProgram> &shaderProgram)
{
    shaderProgram->use();
    model->render();
}

void Renderer::drawScene(Camera *camera, Scene *scene, float currentTime)
{
    Renderer::currentTime = currentTime;

    chag::float4x4 viewMatrix           = camera->getViewMatrix();
    chag::float4x4 projectionMatrix     = camera->getProjectionMatrix();
    chag::float4x4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    // enable back face culling.
    glEnable(GL_CULL_FACE);


    //*************************************************************************
    // Render shadow map
    //*************************************************************************
    chag::float4x4 lightMatrix = chag::make_identity<chag::float4x4>();

    if (scene->shadowMapCamera != NULL) {
        chag::float4x4 lightViewMatrix           = scene->shadowMapCamera->getViewMatrix();
        chag::float4x4 lightProjectionMatrix     = scene->shadowMapCamera->getProjectionMatrix();
        chag::float4x4 lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;

        lightMatrix = chag::make_translation(chag::make_vector( 0.5f, 0.5f, 0.5f ))
                    * chag::make_scale<chag::float4x4>(chag::make_vector(0.5f, 0.5f, 0.5f))
                    * lightViewProjectionMatrix
                    * inverse(viewMatrix);

        drawShadowMap(sbo, lightViewProjectionMatrix, scene);
    }

    //*************************************************************************
    // Render the scene from the cameras viewpoint, to the default framebuffer
    //*************************************************************************
    glBindFramebuffer(GL_FRAMEBUFFER, postProcessFbo.id);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int w = Globals::get(Globals::Key::WINDOW_WIDTH);
    int h = Globals::get(Globals::Key::WINDOW_HEIGHT);
    glViewport(0, 0, w, h);
    // Use shader and set up uniforms
    shaderProgram->use();

    shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 0 * sizeof(chag::float4x4), sizeof(chag::float4x4), &(viewMatrix.c1.x));
    shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 1 * sizeof(chag::float4x4), sizeof(chag::float4x4), &(projectionMatrix.c1.x));
    shaderProgram->setUniformBufferSubData( UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 2 * sizeof(chag::float4x4), sizeof(chag::float4x4), &(viewProjectionMatrix.c1.x));

    //Sets matrices
    shaderProgram->setUniformMatrix4fv("lightMatrix", lightMatrix);
    shaderProgram->setUniformMatrix4fv("inverseViewNormalMatrix", transpose(viewMatrix));
    shaderProgram->setUniform3f("viewPosition", camera->getPosition());
    shaderProgram->setUniformMatrix4fv("viewMatrix", viewMatrix);
    shaderProgram->setUniform1f("time", currentTime);

    setWind(shaderProgram);

    setLights(shaderProgram, scene);

    setFog(shaderProgram);

    //Set shadowmap
    if (scene->shadowMapCamera != NULL) {
        shaderProgram->setUniform1i("has_shadow_map", 1);
        shaderProgram->setUniform1i("shadowMap", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, sbo.texture);
    }

    //Set cube map
    if (scene->cubeMap != nullptr) {
        shaderProgram->setUniform1i("hasCubeMap", 1);
        shaderProgram->setUniform1i("cubeMap", 2);
        glActiveTexture(GL_TEXTURE2);
        scene->cubeMap->bind(GL_TEXTURE2);
    } else {
        shaderProgram->setUniform1i("hasCubeMap", 0);
        shaderProgram->setUniform1i("cubeMap", 2);
    }

    drawShadowCasters(shaderProgram, scene);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawTransparent(shaderProgram, scene);
    glDisable(GL_BLEND);

    drawBloom(emissiveShader, scene, w, h, viewProjectionMatrix);
    renderPostProcess();

    //Cleanup
    glUseProgram(0);

}

void Renderer::setWind(std::shared_ptr<ShaderProgram> shaderProgram) {
    if(currentTime - lastWindChangeTime > 1.0f) {
        lastWindChangeTime += 1;
        lastWindSpeed = newWindSpeed;
        float x = getRand(0, 1);
        float y = getRand(0, 1);
        float z = getRand(0, 1);

        newWindSpeed.x = x * 2.0f - 1.0f;
        newWindSpeed.y = y * 2.0f - 1.0f;
        newWindSpeed.z = z * 2.0f - 1.0f;

    }

    currentWindSpeed = linearSmoothStep(lastWindSpeed, newWindSpeed, currentTime - lastWindChangeTime);
    effects.wind.force = currentWindSpeed;
    shaderProgram->setUniform3f("windForce", effects.wind.force);
}

void Renderer::setLights(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene) {
    //set dirlights
    if(scene->directionalLight) {
        shaderProgram->setUniform3f("directionalLight.colors.ambientColor", scene->directionalLight->ambientColor);
        shaderProgram->setUniform3f("directionalLight.colors.diffuseColor", scene->directionalLight->diffuseColor);
        shaderProgram->setUniform3f("directionalLight.colors.specularColor", scene->directionalLight->specularColor);
        shaderProgram->setUniform3f("directionalLight.direction", scene->directionalLight->direction);
    }

    //set pointLights

    shaderProgram->setUniform1i("nrPointLights", (int)scene->pointLights.size());
    for (int i = 0; i < (int)scene->pointLights.size(); i++) {
        std::string name = std::string("pointLights[") + patch::to_string(i).c_str() + "]";
        shaderProgram->setUniform3f((name + ".position").c_str(), scene->pointLights[i]->position);
        shaderProgram->setUniform3f((name + ".colors.ambientColor").c_str(), scene->pointLights[i]->ambientColor);
        shaderProgram->setUniform3f((name + ".colors.diffuseColor").c_str(), scene->pointLights[i]->diffuseColor);
        shaderProgram->setUniform3f((name + ".colors.specularColor").c_str(), scene->pointLights[i]->specularColor);
        shaderProgram->setUniform1f((name + ".attenuation.constant").c_str(), scene->pointLights[i]->attenuation.constant);
        shaderProgram->setUniform1f((name + ".attenuation.linear").c_str(), scene->pointLights[i]->attenuation.linear);
        shaderProgram->setUniform1f((name + ".attenuation.exp").c_str(), scene->pointLights[i]->attenuation.exp);
    }

    //set spotLights
    shaderProgram->setUniform1i("nrSpotLights", (int)scene->spotLights.size());
    for (int i = 0; i < (int)scene->spotLights.size(); i++) {
        std::string name = std::string("spotLights[") + patch::to_string(i).c_str() + "]";
        shaderProgram->setUniform3f((name + ".position").c_str(), scene->spotLights[i]->position);
        shaderProgram->setUniform3f((name + ".colors.ambientColor").c_str(), scene->spotLights[i]->ambientColor);
        shaderProgram->setUniform3f((name + ".colors.diffuseColor").c_str(), scene->spotLights[i]->diffuseColor);
        shaderProgram->setUniform3f((name + ".colors.specularColor").c_str(), scene->spotLights[i]->specularColor);
        shaderProgram->setUniform1f((name + ".attenuation.constant").c_str(), scene->spotLights[i]->attenuation.constant);
        shaderProgram->setUniform1f((name + ".attenuation.linear").c_str(), scene->spotLights[i]->attenuation.linear);
        shaderProgram->setUniform1f((name + ".attenuation.exp").c_str(), scene->spotLights[i]->attenuation.exp);
        shaderProgram->setUniform3f((name + ".direction").c_str(), scene->spotLights[i]->direction);
        shaderProgram->setUniform1f((name + ".cutoff").c_str(), scene->spotLights[i]->cutOff);
        shaderProgram->setUniform1f((name + ".cutoffOuter").c_str(), scene->spotLights[i]->outerCutOff);
    }
}

/**
* In this function, add all scene elements that should cast shadow, that way
* there is only one draw call to each of these, as this function is called twice.
*/
void Renderer::drawShadowCasters(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene) {
    std::vector<std::shared_ptr<GameObject>> shadowCasters = scene->getShadowCasters();

    for(auto shadowCaster : shadowCasters) {
        shaderProgram->setUniform1f("object_reflectiveness", shadowCaster->shininess);
        drawModel(shadowCaster, shaderProgram);
    }
}

void Renderer::drawTransparent(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene) {
    shaderProgram->use();
    std::vector<std::shared_ptr<GameObject>> transparentObjects = scene->getTransparentObjects();

    for (auto transparentObject : transparentObjects) {
        glEnable(GL_BLEND);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shaderProgram->setUniform1f("object_reflectiveness", transparentObject->shininess);
        drawModel(transparentObject, shaderProgram);
    }
}

void Renderer::drawBloom(std::shared_ptr<ShaderProgram> &shaderProgram, Scene *scene, int w, int h, chag::float4x4 &viewProjectionMatrix) {

    if(!effects.bloom.active){ return; }

    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, verticalBlurFbo.id);
    glActiveTexture(GL_TEXTURE0);
    glViewport(0,0,w,h);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    shaderProgram->use();

    shaderProgram->setUniformMatrix4fv("viewProjectionMatrix", viewProjectionMatrix);

    std::vector<std::shared_ptr<GameObject>> shadowCasters = scene->getShadowCasters();
    for (unsigned int i = 0; i < scene->getShadowCasters().size(); i++) {
        shadowCasters[i]->renderEmissive(shaderProgram);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //HORIZONTAL
    glBindFramebuffer(GL_FRAMEBUFFER, horizontalBlurFbo.id);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    horizontalBlurShader->use();

    horizontalBlurShader->setUniform1i("frameBufferTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, verticalBlurFbo.texture);
    drawFullScreenQuad();

    //VERTICAL
    glBindFramebuffer(GL_FRAMEBUFFER, postProcessFbo.id);
    glDepthFunc(GL_LESS);

    verticalBlurShader->use();

    verticalBlurShader->setUniform1i("frameBufferTexture", 0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, horizontalBlurFbo.texture);
    drawFullScreenQuad();

    //cleanup
    glUseProgram(currentProgram);
    glDisable(GL_BLEND);

}

void Renderer::drawShadowMap(Fbo sbo, chag::float4x4 viewProjectionMatrix, Scene *scene) {
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

    std::vector<std::shared_ptr<GameObject>> shadowCasters = scene->getShadowCasters();
    for (unsigned int i = 0; i < shadowCasters.size(); i++) {
        sbo.shaderProgram->setUniform1f("object_reflectiveness", (*shadowCasters[i]).shininess);
        (*shadowCasters[i]).renderShadow(sbo.shaderProgram);
    }

    //CLEANUP
    glDisable(GL_POLYGON_OFFSET_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(currentProgram);
}

void Renderer::setFog(std::shared_ptr<ShaderProgram> &shaderProgram) {
    shaderProgram->setUniform1i("fog.iEquation",    effects.fog.fEquation);
    shaderProgram->setUniform1f("fog.fDensity",    effects.fog.fDensity);
    shaderProgram->setUniform1f("fog.fEnd",        effects.fog.fEnd);
    shaderProgram->setUniform1f("fog.fStart",    effects.fog.fStart);
    shaderProgram->setUniform3f("fog.vColor",    effects.fog.vColor);
}

void Renderer::initGL()
{
    /* Workaround for AMD. It might no longer be necessary, but I dunno if we
    * are ever going to remove it. (Consider it a piece of living history.)
    */
    if (!glBindFragDataLocation)
    {
        glBindFragDataLocation = glBindFragDataLocationEXT;
    }

    //*************************************************************************
    //    Load shaders
    //*************************************************************************
    shaderProgram = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "shaders/simple.vert", "shaders/simple.frag");

    emissiveShader = ResourceManager::loadAndFetchShaderProgram(EMISSIVE_SHADER_NAME,"shaders/emissive.vert","shaders/emissive.frag");

    shaderProgram->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);
    shaderProgram->initUniformBufferObject(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, 3 * sizeof(chag::float4x4), UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    CHECK_GL_ERROR();



    //*************************************************************************
    // Generate shadow map frame buffer object
    //*************************************************************************
    Logger::logInfo("Generating OpenGL data.");

    sbo.shaderProgram = ResourceManager::loadAndFetchShaderProgram("SHADOW_SHADER", "shaders/shadowMap.vert", "shaders/shadowMap.frag");

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

    chag::float4 zeros = { 1.0f, 1.0f, 1.0f, 1.0f };
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

    std::string post_fx = "POST_FX_SHADER";
    std::string vert_blur = "VERTICAL_BLUR_SHADER";
    std::string hor_blur = "HORIZONTAL_BLUR_SHADER";
    std::string cutoff = "CUTOFF_SHADER";

    postFxShader         = ResourceManager::loadAndFetchShaderProgram(post_fx,   "shaders/postFx.vert", "shaders/postFx.frag");
    verticalBlurShader   = ResourceManager::loadAndFetchShaderProgram(vert_blur, "shaders/postFx.vert", "shaders/vertical_blur.frag");
    horizontalBlurShader = ResourceManager::loadAndFetchShaderProgram(hor_blur,  "shaders/postFx.vert", "shaders/horizontal_blur.frag");
    cutoffShader         = ResourceManager::loadAndFetchShaderProgram(cutoff,    "shaders/postFx.vert", "shaders/cutoff.frag");

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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    postFxShader->use();
    postFxShader->setUniform1i("frameBufferTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, postProcessFbo.texture);

    postFxShader->setUniform1f("time", currentTime);

    drawFullScreenQuad();
}

void Renderer::drawFullScreenQuad()
{
    static GLuint vertexArrayObject = 0;

    // do this initialization first time the function is called... somewhat dodgy, but works for demonstration purposes
    if (vertexArrayObject == 0)
    {
        glGenVertexArrays(1, &vertexArrayObject);
        static const chag::float2 positions[] = {{-1.0f, -1.0f},
                                                 { 1.0f,  1.0f},
                                                 {-1.0f,  1.0f},

                                                 {-1.0f, -1.0f},
                                                 { 1.0f, -1.0f},
                                                 { 1.0f,  1.0f}};

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

void Renderer::setBackgroundColor(chag::float3 rgb) {
    backgroundColor = rgb;
}
