//
// Created by simon on 2016-02-06.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H

#include <vector>
#include <map>
#include <Dimension.h>

using namespace std;

class Texture;
class HUDGraphic;
class IHudDrawable;

/**
 * A layout is an object that is passed to the HudRenderer to render a menu or UI.
 */
class Layout {

public:

    /**
     * Add a sub-layout. How the child is positioned and its dimensions are
     * decided by the implementing class.
     */
    virtual void addChild(Layout* child);

    /**
     * The function to fetch all GLSquares from this layout and its children
     * based on the position and dimensions given. The parent calculates
     * the dimensions and implementing classes must assume that the dimensions
     * are correct. The parent uses getWidth() and getHeight() to determine
     * the dimensions.
     *
     * This function doesn't need to be overidden but getGLSquares(float layoutXPos,
     * float layoutYPos, float layoutWidth, float layoutHeight, map<string,IHudDrawable*>* list) must be
     * overridden.
     */
    virtual map<string,IHudDrawable*> getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                                           float layoutHeight);

    /**
     *
     * \internal
     *
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth() = 0;
    /**
     * \internal
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight() = 0;

    virtual ~Layout(){}

    /**
     * The function to fetch all GLSquares from this layout and its children
     * based on the position and dimensions given. The parent calculates
     * the dimensions and implementing classes must assume that the dimensions
     * are correct. The parent uses getWidth() and getHeight() to determine
     * the dimensions.
     *
     * The overridden method should always call Layout::getGLSquares(float layoutXPos,
     * float layoutYPos, float layoutWidth, float layoutHeight, map<string,IHudDrawable*>* list)
     * before doing anything else. Otherwise Layout::setBackground() won't have an effect.
     */
    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, map<string,IHudDrawable*>* list) = 0;

    /**
     * Sets the background of this Layout.
     */
    virtual Layout* setBackground(HUDGraphic* graphic);

    /**
     * Sets the layout id of this layout. This makes it possible to retrieve the
     * layout by HudRenderer::getLayoutById(). You can also get the rendering GLSquare
     * of the background using HudRenderer::getHudDrawableById().
     */
    virtual Layout* setLayoutId(string id);

    /**
     * Searches through this object and all children after the
     * given ID and returns the first match found.
     * Returns {\code nullptr} if no child had the specified id.
     */
    virtual Layout* findById(string id);

protected:
    vector<Layout*> children;
    HUDGraphic* graphic = nullptr;
    string id = "";

    string getNextRandId();

};

#endif //SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
