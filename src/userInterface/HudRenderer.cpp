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
#include <RelativeIHudDrawable.h>
#include "Dimension.h"

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
    squares = map<string,IHudDrawable*>();
    for(auto relLayout : relativeLayouts){
        map<string,IHudDrawable*> tempSquares = relLayout.second->getGLSquares(0,0,relLayout.second->getWidth().getSize(w),relLayout.second->getHeight().getSize(h));
        for(auto square : tempSquares)
            squares.insert(squares.end(),pair<string,IHudDrawable*>(square.first,new RelativeIHudDrawable(worldCamera,relLayout.first,square.second)));
    }
    rootLayout->getGLSquares(0,0,(float)w,(float)h, &squares);

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

void HudRenderer::setWorldCamera(Camera *worldCamera) {
    this->worldCamera = worldCamera;
}

void HudRenderer::addRelativeLayout(GameObject *relativeTo, Layout *layout) {
    this->relativeLayouts.push_back(pair<GameObject*,Layout*>(relativeTo,layout));
    updateLayout();
}


