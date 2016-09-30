//
// Created by johan on 2015-12-12.
//

#include <ResourceManager.h>
#include <GLSquare.h>
#include <Layout.h>
#include <Globals.h>
#include "HudRenderer.h"
#include <vector>
#include <IHudDrawable.h>
#include <HUDGraphic.h>
#include <SFML/Window/Mouse.hpp>

using namespace std;

HudRenderer::HudRenderer(){

    ResourceManager::loadShader("shaders/hud.vert", "shaders/hud.frag", "hudShader");
    shaderProgram = ResourceManager::getShader("hudShader");

}

void HudRenderer::setLayout(Layout *layout) {
    rootLayout = layout;
    updateLayout();
}

Layout* HudRenderer::getLayoutById(string id) {
    return rootLayout->findById(id);
}

void HudRenderer::updateLayout() {
    int w = Globals::get(Globals::WINDOW_WIDTH), h = Globals::get(Globals::WINDOW_HEIGHT);
    squares = rootLayout->getGLSquares(0,0,(float)w,(float)h);
}

void HudRenderer::render() {
	
    float4x4 orthographicProjection = createOrthographicProjection();
    for(auto child : squares)
        child.second->render(shaderProgram,&orthographicProjection);
		
}

IHudDrawable* HudRenderer::getHudDrawableById(string id) {
    map<string,IHudDrawable*>::iterator it = squares.find(id);
    return it == squares.end() ? nullptr : it->second;
}

float4x4 HudRenderer::createOrthographicProjection() {
    static int w = -1, h = -1;
    static float4x4 orthoMatrix;
    if(Globals::get(Globals::WINDOW_WIDTH) != w || Globals::get(Globals::WINDOW_HEIGHT) != h){
        w = Globals::get(Globals::WINDOW_WIDTH);
        h = Globals::get(Globals::WINDOW_HEIGHT);
        float r = (float)w, l = 0.0f, b = -(float)h, t =  0.0f, f = 1.0f, n = -1.0f;
        orthoMatrix = make_ortho(r,l,t,b,f,n);
    }
    return orthoMatrix;
}

HudRenderer::~HudRenderer(){

}


void HudRenderer::renderShadow(ShaderProgram *shaderProgram) {}

void HudRenderer::update(float dt){
    int x = Globals::get(Globals::MOUSE_WINDOW_X);
    int y = Globals::get(Globals::MOUSE_WINDOW_Y);
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        rootLayout->invokeListenersInternal(x,y,Layout::ON_CLICK_LISTENER,true);
    else
        rootLayout->invokeListenersInternal(x,y,Layout::ON_CLICK_LISTENER,false);
    rootLayout->invokeListeners(x,y,Layout::ON_HOVER_LISTENER);
}