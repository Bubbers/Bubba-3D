//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_HUDRENDERER_H
#define BUBBA_3D_HUDRENDERER_H

#include "IRenderComponent.h"
#include "GL/glew.h"
#include "Utils.h"
#include <map>

using namespace std;

class Texture;
class Layout;
class GLSquare;
class IHudDrawable;

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
    virtual Layout* getLayoutById(string id);

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
    virtual IHudDrawable* getHudDrawableById(string id);

protected:
    float4x4 createOrthographicProjection();
    map<string,IHudDrawable*> squares;
    Layout* rootLayout;

};

#endif //BUBBA_3D_HUDRENDERER_H
