//
// Created by simon on 2016-02-06.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
#define SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H

#include <vector>
#include <map>
#include <Dimension.h>
#include <functional>

using namespace std;

class Texture;
class HUDGraphic;
class IHudDrawable;
class InsideChecker;
class GLSquare;

/**
 * A layout is an object that is passed to the HudRenderer to render a menu or UI.
 */
class Layout {

public:

    enum ListenerType {ON_CLICK_LISTENER,ON_HOVER_LISTENER};

    typedef function<void (int x, int y, Layout* element, bool enteredElseLeaving)> EventFunction;

    /**
     * Add a sub-layout. How the child is positioned and its dimensions are
     * decided by the implementing class.
     */
    virtual void addChild(Layout* child);

    virtual void clearChildren();

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

    virtual void invokeListeners(int x, int y, ListenerType listenerType);
    virtual void invokeListenersInternal(int x, int y, ListenerType listenerType, bool mayBeHit);

    virtual Layout* addClickListener(EventFunction eventFunction);
    virtual Layout* addHoverListener(EventFunction eventFunction);
    virtual void clearHoverListeners();
    virtual void clearClickListeners();
    virtual void callListeners(int x, int y, ListenerType listenerType, bool enteringElseLeaving);
    virtual HUDGraphic* getGraphic();
    virtual void updateGraphic();

protected:
    vector<Layout*> children;
    HUDGraphic* graphic = nullptr;
    string id = "";
    InsideChecker* insideChecker = nullptr;
    vector<EventFunction>* hoverListeners = new vector<EventFunction>();
    vector<EventFunction>* clickListeners = new vector<EventFunction>();
    GLSquare* renderedBackground = nullptr;
    bool hoveredBackground = false;
    bool mouseWasDown = false;

    string getNextRandId();

};

#endif //SUPER_BUBBA_AWESOME_SPACE_LAYOUT_H
