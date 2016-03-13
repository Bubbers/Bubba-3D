//
// Created by simon on 2015-12-29.
//

#ifndef BUBBA_3D_MOUSEPOSITION_H
#define BUBBA_3D_MOUSEPOSITION_H

/**
 * A basic class which makes it easy to find the mouse position based on
 * different relative views. Note that you have to create a new MousePosition object
 * or call object to get the coordinates after the mouse has moved.
 *
 */
class MousePosition {
public:
    /**
     * Fetches the current mouse position and stores it so
     * it can be used with the get functions
     */
    MousePosition();

    /**
     * Fetches the current mouse position and stores it so
     * it can be used with the get functions
     */
    void update();

    /**
     * Returns the raw x position on the whole screen (not just the window)
     */
    int getRawX();

    /**
     * Returns the raw y position on the whole screen (not just the window)
     */
    int getRawY();

    /**
     * Returns the x position relative to the left side of the window
     */
    int getWindowX();

    /**
     * Returns the y position relative to the upper side of the window
     */
    int getWindowY();
    /**
     * Gets the x offset from the window center.
     */
    int getXMidOffset();
    /**
     * Gets  the y offset from the window center.
     */
    int getYMidOffset();

private:
    int x,y,wx,wy;

};


#endif //BUBBA_3D_MOUSEPOSITION_H
