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

#include "IRenderComponent.h"
#include "GL/glew.h"
#include "Utils.h"
#include <map>

class Texture;
class Layout;
class GLSquare;
class IHudDrawable;
class Camera;

/**
 * The main class used to render a HUD or other types of UI
 */
class HudRenderer : public IRenderComponent
{
public:
    HudRenderer();
    ~HudRenderer();

    virtual void render();
    virtual void renderShadow(ShaderProgram *shaderProgram);
    virtual void update(float dt);

    /**
     * Whenever you change something about the active Layout or its children
     * you must call this method to update the rendering. This is a bit expensive
     * and should be avoided when possible.
     * \see HudRenderer::getHudDrawableById()
     */
    virtual void updateLayout();

    /**
     * Set the layout of the HUD
     */
    virtual void setLayout(Layout* layout);

    /**
     * Get any layout in the root layout added in HudRenderer::setLayout() by
     * its layout ID.
     */
    virtual Layout* getLayoutById(std::string id);

    /**
     * Get the low-level drawable produced by any of the layout
     * components.
     *
     * You can retrieve the GLSquare produced as a background
     * by specifying the layout id of the Layout that produced the background.
     * This can be useful if you want to modify the rotation and/or position of
     * an element without calling HudRenderer::updateLayout()
     *
     * You can retrieve the TextObject produced by a TextLayout by specifying
     * the text id. This can be used to modify the text of the element without
     * calling HudRenderer::updateLayout()
     * \see Layout::setLayoutId()
     * \see TextLayout::setTextId()
     */
    virtual IHudDrawable* getHudDrawableById(std::string id);

    void setWorldCamera(Camera* worldCamera);

    void addRelativeLayout(GameObject* relativeTo, Layout* layout);

protected:
    chag::float4x4 createOrthographicProjection();
    std::map<std::string, IHudDrawable*> squares;
    Layout* rootLayout;
    Camera* worldCamera;
    std::vector<std::pair<GameObject*,Layout*>> relativeLayouts;

};
